#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/string.h>


#define MYMA  1314
#define COUNT 1

typedef struct {
    struct mutex mutex;
    struct cdev  cdev;
    struct i2c_client *cli; 
    struct class *cls;
}dht12_data_t;


ssize_t dht12_read(struct file *fl, char __user *buf, size_t len, loff_t *off)
{
    struct cdev *cdev = fl->f_path.dentry->d_inode->i_cdev;
    dht12_data_t *data = container_of(cdev, dht12_data_t, cdev);
    int ret, sum, i;
    struct i2c_msg msgs[2];
    char data_rcv[5];
    char addr = 0;
    struct i2c_client *cli = data->cli;

    memset(data_rcv,0,5);
    //上锁，防止重入
    mutex_lock(&data->mutex);
    
    msgs[0].addr = 0x5c;
    msgs[0].flags = 0;
    msgs[0].len = 1;
    msgs[0].buf = &addr;

    msgs[1].addr = 0x5c;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len = 5;
    msgs[1].buf = data_rcv;
    ret = i2c_transfer(cli->adapter, msgs, 2);
    if (ret != 2)
        goto out;
   
    strncpy(buf,data_rcv,5);
    //检查校验和
    sum = 0;
    for (i = 0; i < 4; i++)
        sum += data_rcv[i];
    if (sum != data_rcv[4])
    {
        ret = -EINVAL;
        goto out;
    }
    ret = 5;
out:
    mutex_unlock(&data->mutex);
    return ret;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = dht12_read,
};

int dht12_probe(struct i2c_client *cli, const struct i2c_device_id *id)
{
    static int mi = 0; //记录次设备号
    dht12_data_t *data;
    dev_t devid;
    int ret;

    //注册设备号
    devid = MKDEV(MYMA, mi);
    ret = register_chrdev_region(devid, COUNT, cli->name);
    if (ret < 0)
        goto err0; 

    //准备每个匹配设备的数据
    data = kzalloc(sizeof(*data), GFP_KERNEL);
    if (NULL == data)
    {
        ret = -ENOMEM;
        goto err1;
    }

    //字符设备初始化
    cdev_init(&data->cdev, &fops);
    data->cdev.owner = THIS_MODULE;
    ret = cdev_add(&data->cdev, devid, COUNT);
    if (ret < 0)
        goto err2;

    //初始化互斥锁
    mutex_init(&data->mutex);   

    //创建设备文件
    data->cls = class_create(THIS_MODULE, cli->name);
    device_create(data->cls, NULL, devid, NULL, "%s.%d", cli->name, mi++);
    data->cli = cli;

    i2c_set_clientdata(cli, data); //dev_set_drvdata(&cli->dev, data);
    return 0;
err2:
    kfree(data);
err1:
    unregister_chrdev_region(devid, COUNT);
err0:
    return ret;
}

int dht12_remove(struct i2c_client *cli)
{
    dht12_data_t *data = i2c_get_clientdata(cli); //dev_get_drvdata(&cli->dev);

    //移除设备文件
    device_destroy(data->cls, data->cdev.dev);
    class_destroy(data->cls);

    cdev_del(&data->cdev);
    unregister_chrdev_region(data->cdev.dev, COUNT);
    kfree(data);
    return 0;
}


struct of_device_id dht12_dt_match[] = {
    {.compatible = "dht12"},
    {},
};
struct i2c_device_id dht12_ids[] = {
    {"dht12", 0},
    {},
};
struct i2c_driver dht12_drv = {
    .probe = dht12_probe,
    .remove = dht12_remove,

    .driver = {
        .name = "i2c-slave-dht12",
        .owner = THIS_MODULE,
    },

    .id_table = dht12_ids,
};


module_i2c_driver(dht12_drv);
MODULE_LICENSE("GPL");

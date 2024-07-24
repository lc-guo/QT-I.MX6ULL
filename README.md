# 基于 QT 的嵌入式 Linux 驱动综合测试及应用平台

## 0、简介

本项目是以 I.MX6ULL 为硬件平台，基于 Qt 搭建的用于 Linux 各类设备驱动及应用的综合测试平台，包括 platform、I2C、SPI 和 V4L2 等设备驱动框架测试。

目前已经完成如下所示 10+ 应用：

1. UVC 相机
2. 画图板
3. 姿态角展示
4. 环境光监测
5. ADC 采集
6. 设置
7. CAN 接收测试
8. UART 接收测试
9. 音乐播放器
10. 视频播放器（BUG版）
11. GPIO 输出测试
12. ...


视频演示

[基于 Qt 的嵌入式 Linux 驱动综合测试及应用平台](https://www.bilibili.com/video/BV1fpb1edEFf/?share_source=copy_web&vd_source=7bc1e04febb8764f047d17d062e1688d)

## 1、拉取仓库代码

拉取仓库代码，文件系统大概 550MB，因此需要拉取大文件，按照下面步骤操作即可

```bash
# 克隆仓库
git clone git@github.com:lc-guo/QT-I.MX6ULL.git
# 进入仓库目录
cd QT-I.MX6ULL
# 安装 LFS
git lfs install
# 拉取大文件
git lfs pull
```

由于 GitHub LFS 免费用户每月只有 1GB 下载配额（果然无法白嫖），因此项目大家也可以在如下的百度云盘链接中下载

[QT-I.MX6ULL](https://pan.baidu.com/s/1xRJXqCw2X0s4M2jTYnzlpQ?pwd=cxmi)

## 2、制作 SD 卡启动盘

制作 SD 卡启动盘，SD 卡应该有 3 个分区，并且分区中存放的内容一致

1. 分区 0 对应 sdx，用于存放 u-boot.imx
2. 分区 1 对应 sdx1，格式为 vfat，用于存放 Linux 镜像和 .dtb 设备树文件
3. 分区 2 对应 sdx2，格式为 ext4，用于存放根文件系统（rootfs）

首先在 windwos 下格式化整个 SD 卡，进入 CMD ，输入以下命令对 SD 卡格式化

```bash
# 进入磁盘管理
diskport
# 列举所有磁盘
list disk
# 选择 SD 卡磁盘
select disk n
# 清除磁盘内容
clean
# 创建原始分区
create partition primary
# 格式化为 FAT32 格式
format fs=fat32 quick
```

然后将 SD 卡插入 Linux 服务器，使用 `fdisk` 命令制作 SD 卡启动盘

```bash
# 使用 fdisk 命令修改 SD 卡分区
sudo fdisk /dev/sda
# 删除当前分区，删除后 SD 卡应该没有任何分区
d
# 创建新主分区，分区号为 1 ，起始扇区为 20480 ，分区大小为 500M
n
p
1
20480
+500M
# 查看新创建的分区
p
# 创建新主分区，分区号为 2 ，起始扇区为 1044480
n
p
2
1044480
<enter>
# 查看新创建的分区
p
# 将分区表写入磁盘并退出
w
```

创建好分区之后，需要对分区进行格式化操作

```bash
# 将分区 sdb1 格式化为 vfat
sudo mkfs -t vfat /dev/sdb1
# 将分区 sdb2 格式化为 ext4
sudo mkfs -t ext4 /dev/sdb2
```

最终 SD 卡内部分区应该如下图所示

![linux_sd_startup.png](https://github.com/lc-guo/QT-I.MX6ULL/blob/main/linux_sd_startup.png)

将 U-Boot 目录中的 `u-boot.bin` 使用 `imxdownload` 工具烧录到 SD 卡的第一个分区

```bash
# 解压
tar -xvf U-Boot.tar.gz
# 进入目录
cd U-Boot
# 烧录 U-Boot 
./imxdownload u-boot.bin /dev/sdx
```

将 kernel 目录中的镜像和设备树拷贝到 SD 卡的第二个分区

```bash
# 解压
tar -xvf kernel.tar.gz
# 创建挂载文件夹
sudo mkdir tempdir
# 挂载 SD 卡第二个分区
sudo mount /dev/sdx1 tempdir/
# 拷贝镜像和设备树
sudo cp kernel/* tempdir/
# 卸载 SD 卡
sudo umount tempdir
```

Linux 内核版本为 4.1.15 ，是在 NXP 官方 MCIMX6ULL-EVK 开发板上修改得到的，具体可以学习正点原子的教程

[nxp-imx/linux-imx at imx_4.1.15_2.0.0_ga](https://github.com/nxp-imx/linux-imx/tree/imx_4.1.15_2.0.0_ga)

将 rootfs 目录中的根文件系统拷贝到 SD 卡的第三个分区

```bash
# 解压
tar -xvf rootfs.tar.gz
# 创建挂载文件夹
sudo mkdir tempdir
# 挂载 SD 卡第三个分区
sudo mount /dev/sdx2 tempdir/
# 拷贝根文件系统
sudo cp -r rootfs/* tempdir/
# 卸载 SD 卡
sudo umount tempdir
```

根文件系统由 BusyBox 构建，已经移植好了 Qt5.14.2、tslib、MPlayer 和 一些硬件驱动，所有使用到的驱动全部在根文件系统 `/lib/modules/4.1.15/` 目录下，Qt5.14.2 和 tslib 库位于 `/usr/lib` 目录下

## 3、从 SD 卡启动开发板

将启动方式拨码开关设置为从SD卡启动，插入SD卡然后打开电源，观察终端输出，一切正常会自动启动桌面程序

如果无法启动，应用程序位于 `/qt/` 目录，名为 `IMX6ULL` ，可以尝试手动启动

# 4、参考资料

1.  [IMX6ULL-QT项目之mplayer音乐播放器](https://blog.csdn.net/qq_40792874/article/details/120564555)
2. [【正点原子】I.MX6U嵌入式Qt开发指南V1.1](https://gitee.com/GuangzhouXingyi/imx6ull-document)
3. [【正点原子】I.MX6U嵌入式Linux驱动开发指南V1.81](https://gitee.com/GuangzhouXingyi/imx6ull-document)
4. [Linux相机小项目](https://gitee.com/chenshao777/linux-camera-mini-project)

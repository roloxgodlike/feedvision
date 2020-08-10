# HCNetCamera
海康威视网络摄像头

<h2 id=install>软件安装步骤</h2>

1. 安装git

如果系统中已经安装了git，此步可以跳过。

如果系统是CentOS，请执行以下指令（需要root权限）：
```shell
yum -y install git
```

如果系统是Ubuntu，请执行以下指令（需要root权限）：
```shell
apt-get install git
```

2. 安装编译器

如果系统中已经安装了g++编译器，此步可以跳过。

如果系统是CentOS，请执行以下指令（需要root权限）：
```shell
yum -y install gcc-c++
```

如果系统是Ubuntu，请执行以下指令（需要root权限）：
```shell
apt-get install g++
```

3. 下载软件

使用以下指令下载软件。注意：此仓库是私有仓库，默认无法访问。如需下载软件，请与pk@pkemb.com联系。

```shell
cd ~
git clone https://github.com/pkemb/HCNetCamera.git
```

4. 编译软件

使用以下指令编译软件。编译成功后会生成文件`lib/HCNetCamera`。如果编译过程中遇到任何问题，请与pk@pkemb.com联系，或者提交issue。

```shell
cd HCNetCamera
make
```

<h2 id=install>软件使用方法</h2>

本小节假设软件已经下载完毕，并编译成功。并且当前所在的文件夹是`~/HCNetCamera/lib`。

由于SDK库的一些限制，必须在SDK库所在的目录执行程序，一些动态库才能够正确的加载。

1. 使用帮助

执行`./HCNetCamera -h`可以查看帮助信息。下面是示例输出。请以实际执行的结果为准，下面的内容可能过时了。

```
Usage: ./HCNetCamera OPTIONS
长选项的强制性参数对于短选项也是必需的。
必须选项：
  -a, --address=地址        网络摄像头的地址
  -p, --port=端口           网络摄像头的端口
  -u, --username=用户名     网络摄像头的用户名
  -w, --password=密码       网络摄像头的密码

可选选项：
  -t, --time-interval=time  抓图时间间隔，取值需要大于0，默认是60s
  -d, --dir=dir             图像的存储目录，默认是./pic
  -q, --pic-quality=quality 图片质量系数，0 最好，1 较好，2 一般。默认值为2。
                            若参数非法，则取值为2。
  -s, --pic-size=size       图片尺寸，详细请参考SDK手册或README。默认值为0。
                            如果参数非法，或者摄像头不支持，SDK会自动选择一个默认值。
  -h, --help                打印帮助信息，并终止程序


示例：
  ./HCNetCamera -a 127.0.0.1 -p 12306 -u root -w 123456
  ./HCNetCamera --address=127.0.0.1 --port=12306 --username=root --password=123456
```

2. 图片尺寸

下表是`--pic-size`支持的数值，及其对应的图片大小。注：对于某一具体型号的NVR，只会支持启动一部分参数。如果所给的参数不支持，SDK会使用默认值`255`。

<table>
 <tr>
  <td>--pic-size</td><td>尺寸</td>
  <td>--pic-size</td><td>尺寸</td>
 </tr>
 <tr>
  <td>0</td><td>CIF(352*288/352*240)</td>
  <td>27</td><td>1600*600</td>
 </tr>
 <tr>
  <td>1</td><td>QCIF(176*144/176*120)</td>
  <td>28</td><td>2048*768</td>
 </tr>
 <tr>
  <td>2</td><td>4CIF(704*576/704*480)或D1(720*576/720*486)</td>
  <td>29</td><td>160*120</td>
 </tr>
 <tr>
  <td>3</td><td>UXGA(1600*1200)</td>
  <td>75</td><td>336*256</td>
 </tr>
 <tr>
  <td>4</td><td>SVGA(800*600)</td>
  <td>78</td><td>384*256</td>
 </tr>
 <tr>
  <td>5</td><td>HD720P(1280*720)</td>
  <td>79</td><td>384*216</td>
 </tr>
 <tr>
  <td>6</td><td>VGA(640*480)</td>
  <td>80</td><td>320*256</td>
 </tr>
 <tr>
  <td>7</td><td>XVGA(1280*960)</td>
  <td>82</td><td>320*192</td>
 </tr>
 <tr>
  <td>8</td><td>HD900P(1600*900)</td>
  <td>83</td><td>512*384</td>
 </tr>
 <tr>
  <td>9</td><td>HD1080P(1920*1080)</td>
  <td>127</td><td>480*272</td>
 </tr>
 <tr>
  <td>10</td><td>2560*1920</td>
  <td>128</td><td>512*272</td>
 </tr>
 <tr>
  <td>11</td><td>1600*304</td>
  <td>161</td><td>288*320</td>
 </tr>
 <tr>
  <td>12</td><td>2048*1536</td>
  <td>162</td><td>144*176</td>
 </tr>
 <tr>
  <td>13</td><td>2448*2048</td>
  <td>163</td><td>480*640</td>
 </tr>
 <tr>
  <td>14</td><td>2448*1200</td>
  <td>164</td><td>240*320</td>
 </tr>
 <tr>
  <td>15</td><td>2448*800</td>
  <td>165</td><td>120*160</td>
 </tr>
 <tr>
  <td>16</td><td>XGA(1024*768)</td>
  <td>166</td><td>576*720</td>
 </tr>
 <tr>
  <td>17</td><td>SXGA(1280*1024)</td>
  <td>167</td><td>720*1280</td>
 </tr>
 <tr>
  <td>18</td><td>WD1(960*576/960*480),<span
  style='mso-spacerun:yes'>&nbsp;</span></td>
  <td>168</td><td>576*960</td>
 </tr>
 <tr>
  <td>19</td><td>1080I(1920*1080)</td>
  <td>180</td><td>180*240</td>
 </tr>
 <tr>
  <td>20</td><td>576*576</td>
  <td>181</td><td>360*480</td>
 </tr>
 <tr>
  <td>21</td><td>1536*1536</td>
  <td>182</td><td>540*720</td>
 </tr>
 <tr>
  <td>22</td><td>1920*1920</td>
  <td>183</td><td>720*960</td>
 </tr>
 <tr>
  <td>23</td><td>320*240</td>
  <td>184</td><td>960*1280</td>
 </tr>
 <tr>
  <td>24</td><td>720*720</td>
  <td>185</td><td>1080*1440</td>
 </tr>
 <tr>
  <td>25</td><td>1024*768</td>
  <td>500</td><td>384*288</td>
 </tr>
 <tr>
  <td>26</td><td>1280*1280</td>
  <td>255</td><td>Auto(使用当前码流分辨率)</td>
 </tr>
</table>

3. 快速执行示例

由于需要指定较多的参数，`HCNetCamera`目录下有一个快速执行的示例脚本`run.sh`。可用以下指令快速启动程序。如需不同的参数，只要更改脚本的内容即可。

```shell
./run.sh
```
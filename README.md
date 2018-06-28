# TEmu

#### 交易网关C++客户端Demo源代码
　TEmu是C++客户端Demo源代码，用于演示如何连接蓝目行情数据网关和交易网关，以统一化的数据格式，执行各个交易所的委托下单、委托撤单、交易记录查询等接口，TEmu支持32bit或64bit编译方式。数据货币爱好者可以将此代码结合到自己的策略中，获取多个市场的行情并进行交易。<br>
　蓝目数据提供了全球各大主流数字资产交易所的全部品种的实时交易行情和各类历史数据，供专业的投资机构和个人宽客使用。数据包含了实时数据ticker, 深度数据depth, 各周期历史kline，并且提供了各个交易所的委托下单、委托撤单、交易信息查询等接口。<br>
　目前支持的数字资产交易所包括：bitfinex, kraken, bitstamp, bittrex, okex, huobi, binance, bitmex,zb, poloniex, bithumb. 提供详细的API访问接口文档，以及针对python, C++ 和java 的客户端访问源码。<br><br>
　[官方网站](https://www.blueye.info)

#### 特别说明
　在此做个特别的说明：使用TEmu客户端前，请先确认您需要接入的行情和交易网关。行情网关可以使用蓝目的测试云主机，您也可以通过蓝目网站“申请接入”链接，申请网关token后，在自己的云主机或者本地搭建网关；交易网关由于涉及到您的个人账户安全性,建议按照“申请接入”及交易网关的部署流程，在您自己的云主机或者本地搭建。<br>
　网关更详细的信息请直接到　[官方网站](https://www.blueye.info)

#### 使用对象
　数字货币数据分析师<br>
　数字货币量化交易的个人或团队<br>
　对数字货币交易感兴趣的机构<br>
　分析行情数据的个人或团队<br>
　正在学习数字货币交易的人<br>
#### 使用前提
　安装visual studio c++开发工具，建议安装vs2017，其他如2010及以上版本，则需要下载对应的boost、openssl库文件。<br>
　安装boost 1.67.0。<br>
　安装openssl 1.0.2。<br>
　安装行情或交易网关。<br>

#### 下载安装
1. 下载安装visual studio c++开发工具，以下以vs2017为例，其他版本注意使用boost和openssl的对应版本。<br>
略过

2. 下载安装boost库<br>
下载boost 1.67.0 版本，建议使用二进制版本，无需编译。 https://dl.bintray.com/boostorg/release/1.67.0/binaries/
与vc2017匹配的32位版本是： boost_1_67_0-msvc-14.1-32.exe  
选好安装目录，下一步直到结束。

3. 下载安装openssl库<br>
在 http://slproweb.com/products/Win32OpenSSL.html 下载Win32 OpenSSL v1.0.2o。安装过程中选择将openssl dll复制到openssl bin目录下。

4. 下载Temu代码

5. 设置vc2017选项（debug版本）<br>
   1. C/C++ | 常规 | 附加包含目录：E:\dev\boost_1_67_0;E:\dev\openssl-win32-1_0_2\include，设置为实际安装目录
   2. 连接器  | 常规 | 附加库目录： E:\dev\openssl-win32-1_0_2\lib\VC;E:\dev\boost_1_59_0\stage\lib32-msvc-10.0;
   3. 连接器  | 输入  |附加依赖项： libeay32MDd.lib;ssleay32MDd.lib;
   
####  交易测试

[TEmu测试方法.txt](./TEmu测试方法.txt)


####  消息格式文档

[数字货币网关消息定义](./数字货币网关消息定义-20180622.docx)

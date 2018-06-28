# TEmu

#### 交易网关C++客户端Demo
　蓝目数据提供了全球各大主流数字资产交易所的全部品种的实时交易行情和各类历史数据，供专业的投资机构和个人宽客使用。数据包含了实时数据ticker, 深度数据depth, 各周期历史kline，并且提供了各个交易所的委托下单、委托撤单、交易信息查询等接口。<br>
　TEmu是C++客户端Demo，用于连接蓝目交易网关，以统一化的数据格式，执行各个交易所的委托下单、委托撤单、交易记录查询等接口，TEmu支持32bit或64bit编译方式。<br>
　目前支持的数字资产交易所包括：bitfinex, kraken, bitstamp, bittrex, okex, huobi, binance, bitmex,zb, poloniex, bithumb. 提供详细的API访问接口文档，以及针对python, C++ 和java 的客户端访问源码。<br><br>
　[官方网站](https://www.blueye.info)
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

#### 下载安装
1. 下载安装visual studio c++开发工具，以下以vs2017为例，其他版本注意使用boost和openssl的对应版本。<br>
略过

2. 下载安装boost库
下载boost 1.67.0 版本，建议使用二进制版本，无需编译。 https://dl.bintray.com/boostorg/release/1.67.0/binaries/
与vc2017匹配的32位版本是： boost_1_67_0-msvc-14.1-32.exe  
选好安装目录，下一步直到结束。

3. 下载安装openssl库

在 http://slproweb.com/products/Win32OpenSSL.html下载Win32 OpenSSL v1.0.2o。安装过程中选择将openssl dll复制到openssl bin目录下。

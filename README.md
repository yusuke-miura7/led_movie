# LEDパネル制御プロジェクト
このプロジェクトはラズパイとパネルコントローラを用いてledパネルに画像を表示させたり、動画を再生するためのものです。  
  
![IMG20220803231025](https://github.com/yusuke-miura7/test/assets/124438316/b17297fa-e721-4794-aa93-e84f7a3610e7)  
  
 ## 概要
簡単な流れは次の通りです。

1. ラズパイでmicroUSB経由で画像データ（BMPファイル）を高速に読み書きします。
2. SPI通信を使用してLEDパネルコントローラにRGBデータを送信します。
3. パネルコントローラを介してLEDパネルに表示します。
(パネルコントローラとは、HUB75規格のRGBドットマトリクスLEDパネルのハードウェアを隠蔽し、SPI接続で簡単にフルカラー表示を行えるようにしたものです。)

手順は以下の通りです。

1. `write_data2.c` を実行して表示したい動画ファイル（BMP形式の画像データ）を読み込み、各ピクセルのRGB値をテキストファイルに書き込みます。（動画の著作権のため、リポジトリにはアップロードしていませんが、`bmp6432` フォルダに画像データを保存してください。）
2. `img.c` を実行して作成したテキストファイルからデータを読み込み、SPI通信を使用してパネルコントローラに送信します。今回は、BCM2835ライブラリ(ver.1.71)を使用してGPIOピンからSPI通信を実現しました。
  
## 使用した機材

以下の機材を使用しました。

### LEDパネル（64×32）
![IMG20230515165702](https://github.com/yusuke-miura7/test/assets/124438316/62932ecb-7d1a-4222-88ca-e1018384f78f)  
Amazonにて購入 https://www.amazon.co.jp/gp/product/B07SPFK8P1/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1

### Rasberry Pi400  
![IMG20230515165630](https://github.com/yusuke-miura7/test/assets/124438316/3ca0784a-f1fa-4cf8-84c1-e3d7511eb403)  
秋月電子にて購入 https://akizukidenshi.com/catalog/g/gM-16739/  
microSDカードの読み書きが高速に行えるマイコンとして採用。動画を表示させるためには、パネルコントローラに膨大な連続した画像データを高速に処理して送信する必要があります。低スペックなマイコンでも、内部のデータ転送の方法にDMAを使用したり、SDカードをSPIモードで接続するのではなく、SDネィティブモード(4bitバスモード)にするなどして改善方法はあるらしいですが、今回は何より実装したいのが第一だったのでマシンパワーに頼りました。

### パネルコントローラ  
![IMG20230515165857](https://github.com/yusuke-miura7/test/assets/124438316/76e9ef45-6634-47a5-a164-3a3dac8c713a)  
個人で開発されている方から個人的に購入。このコントローラの規格と提供されているリファレンスに基づいてプログラムを作成しました。

### スイッチング電源(5V18A)+片プラグ付耐熱コード  
![IMG20230515165805](https://github.com/yusuke-miura7/test/assets/124438316/879da112-c948-45ad-8708-82c698dbbd8d)  
akiba LED ピカリ館にて購入  https://www.akiba-led.jp/  
LEDに使用する電源は基本的にDC5Vから持ってくる必要があります。  
ピカリ館のホームページではパネルサイズにあった推奨アンペア数を調べることができます。  
将来的にパネルのサイズを大きくした際にも対応できるように、アンペア数に余裕を持たせたものを選びました。

### ジャンプワイヤー  
![IMG20230515165831](https://github.com/yusuke-miura7/test/assets/124438316/44dc7fea-c01e-4be3-8303-c34a98b0b354)  
Amazonにて購入 https://www.amazon.co.jp/gp/product/B07LG6R78R/ref=ppx_yo_dt_b_asin_title_o06_s00?ie=UTF8&psc=1  
SPI通信が不安定になり、うまく動作しなくなることがあるため、束を割かずにそのままジャンプワイヤーを使用するのがおすすめです。

## その他 
### Rasberry Piのピン配置  
![raspberrypi-gpio-04-1](https://github.com/yusuke-miura7/test/assets/124438316/f3906b61-2a20-4f00-8482-6a964b48f4c3)  
画像はRasberyy Pi4のピン配置ですが、400でも基本的に同じでした。  
こちらのピン配置を参考にして、電源やSPI通信で使用するピンとコントローラを接続しました。

### 補足
また、ESP32を使用して無線通信を行い、時刻を取得することで以下のようなものも作成可能です。  
https://github.com/yusuke-miura7/test/assets/124438316/f62ff017-b19f-4916-bd82-88851c8eba69    
このように、無線通信やセンサを通じて得られた情報とパネルを掛け合わせるだけでも様々なものが作れそうです。  
今後はパネルサイズを拡大して大画面にしたり、[音声認識により感情感知ができるAPI](https://webempath.net/lp-jpn/)があるようなのでこれを使用して感情によってパネルに表示させる内容を変えたり、音声出力をしてみたいです。
最近出てきたchatGPTのAPIも使ってみると面白そうです。  
また、様々なIoTを実現できるように、電子回路についてももっと勉強していきたいです。

## 参考サイト  
以下の参考サイトが、SPI通信やBCM2835ライブラリについて理解するのに役立ちました。
- [SPI通信とは](https://www.analog.com/jp/analog-dialogue/articles/introduction-to-spi-interface.html)
- [ラズパイでSPIを使えるようにするための設定](https://101010.fun/iot/adc-max1118-raspberry-pi-zero.html)
- [BCM2835リファレンス](https://www.airspayce.com/mikem/bcm2835/group__constants.html#gaf2e0ca069b8caef24602a02e8a00884e)
- [BCM2835の簡単なプログラム](https://tomosoft.jp/design/?p=5252)
- [Rasberrypi ピン配置](https://www.bioerrorlog.work/entry/raspberry-pi-pinout)

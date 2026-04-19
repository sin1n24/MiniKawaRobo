---
layout: default
title: "つくりかた"
nav_order: 20
---


# ミニかわロボのつくりかた

ミニかわロボは、大きく分けて**5つの要素**で構成されています。

自分の好みや予算に合わせて、パーツを自由に組み合わせて作ることができます！

「なんもわからんが、とにかく作りたいぜ」という人向けに[ワークショップ](workshop.md)実施検討中です。

1. TOC
{:toc}

---

## 🛠 5つの基本要素

### 1. 電池（パワー源）
ロボットを動かすための電源です。サイズと容量のバランスで選びます。

| パーツ例 | 特徴・予算 |
| :--- | :--- |
| **乾電池** | 安全、入手が容易、重い (約200円) |
| **小型リポバッテリ** | 軽量・コンパクト (約500円) |
| **ドローン用バッテリ** | 比較機安全、高出力。充電器セットもある。 (約500～1000円) |
| **バッテリ内蔵マイコン** | コンパクト。M5Capsuleなど。 (約3,850円) |

**リポバッテリを使用する際は充電や配線に充分注意して下さい。**

**未経験の方は、乾電池がおススメです（20gの重量制限緩和もあります）**

### 2. 制御（頭脳）
ロボットの動きを司るマイコン部分です。M5Stackシリーズが通信もしやすく便利です。

* **[M5Atom Lite](https://ssci.to/8778) / [S3](https://ssci.to/8670)** 安価で非常にコンパクト (1,800円〜2,800円)
* **[M5Capsule](https://ssci.to/9272)** バッテリー内蔵でコンパクト。 (約3,850円) 
* **[サーボ接続基板キット](https://ssci.to/11121) / [基板](https://ssci.to/9475)** 上記にプラスして組立すると配線がよりスマートに！

### 3. 操縦（コントローラー）
自分で操縦するためのインターフェースです。

* **スマホアプリ** [`UGOKU Pad`](https://ugoku-lab.github.io/ugokupad.html) や [`Blynk`](https://www.blynk.io/) などで手軽に開始（０円）
* **[小型コントローラ](https://ssci.to/9520) / [キット](https://ssci.to/9521)** ジョイスティックで手ごたえやカスタマイズ可能 (約3,000円＋AtomS3)
* **ラジコン用送受信機** かわロボやラジコン経験者など既にプロポを持っている人向け。

### 4. サーボ（筋肉）
腕を動かしたり、車輪を回したりするためのモーターです。

* **マイクロサーボ ([FS90](https://akizukidenshi.com/catalog/g/g114806/) / [FS90R](https://akizukidenshi.com/catalog/g/g113206/))** 定番のパーツ (3個で約1,500円）より頑丈な[SG92R](https://akizukidenshi.com/catalog/g/g108914/)や[SG90-HV](https://akizukidenshi.com/catalog/g/g114382/)もあり
* **[Servo Kit 360](https://ssci.to/6479)** フレームとタイヤもセットになった便利なキット (約1,700円)

### 5. フレーム（体）
ロボットの見た目と機構そのものです。
タイヤにゴムを貼るのを忘れずに！

* **[3Dプリンタ製フレーム](https://makerworld.com/en/models/795780-mini-kawa-robot#profileId-1113049)** 設計データを公開中
* **自作メカ** 3DCADでオリジナルの構造を設計して、世界に一台の機体をつくろう！
* **レゴ** 上記のServo Kit 360付属フレームはレゴ互換なので、そこから改造する

---

## 📚 もっと詳しく知る

ミニかわロボに関する深いノウハウは、以下のリンク集からも参照できます。

* [**ルール全文・意見募集**](https://sin1n24.github.io/MiniKawaRobo/rules/)
* [**設計・製作・運用のポイント**](https://sin1n24.hatenablog.com/archive/category/ミニかわロボ)

<iframe src="https://hatenablog-parts.com/embed?url=https://sin1n24.hatenablog.com/entry/2023/12/18/000338" 
        style="border: 0; display: block; width: 100%; height: 155px; max-width: 500px; margin: 10px 0px;" 
        frameborder="0" scrolling="no"></iframe>

> だいたい「ミニかわロボ」で検索すれば、最新の技術情報が見つかります！

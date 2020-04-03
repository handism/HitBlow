# HitBlow

## 概要
C言語で作成したWindows向けHit ＆ Blowゲーム。

## 機能
- exeをダブルクリックで起動（要Windows）
- Hit & Blowゲームが可能
    - 数値の数は3つ
    - 数値の範囲は0 ～ 9
    - 数値の重複はなし
    - 回答数は5回

## 使用技術
- 内部仕様：C言語
- GUI：WIN32API

## コンパイルコマンド

>gcc HitBlow.c -mwindows

## 注意点
- 文字コードは「Shift JIS」じゃないと文字化けするので注意
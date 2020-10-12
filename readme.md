# Genetic Algorithm

- 昨年作ったテトリスの敵プレイヤーのブロック配置方法の評価を遺伝的アルゴリズムに置き換えたもの。

## 遺伝的アルゴリズムの評価

- 操作可能なブロックを任意の場所に配置した際のゲーム盤の状態を評価する。深度が設定されている場合、任意の数のブロックを配置後のゲーム盤の状態を評価する。
- 評価関数は5つありその5つの関数の合計が最も小さいものが最善の手と評価する。5つの評価関数の説明は以下に示す。
    - 分散: 各列の最大高度の分散値
    - 空白数: 各列の最大高度より下にある空白数
    - 最大高度: 最も高い列の高度
    - 最大高度差: 隣接する列の高度差の最大値
    - 消去行数: 削除した列数(多く削除したほうが値が低くなるように処理)
- 5つの各評価値に遺伝的アルゴリズムで生成した値を重みとして掛け、その和が評価結果である。(例えば、空白数の重みが100、他の重みが1の場合、空白数を重視した評価となる)
- 重みは0~99の値とし、この各評価関数の重みを変数として与える。
- 重みの評価は、使用したブロック数と、各ラインの倍率を各ラインの消去数で掛けた値を、足した値となる。

## 実行環境

- os: Ubuntu 20.04.1 LTS (64-bit)
- コンパイラ: g++\-9 (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
- cpp言語バージョン: c++17

## 外部ライブラリ

- libboost-all-dev 1.71.0.0ubuntu2 amd64 Boost C++ Libraries development files (ALL) (default version)
- libsdl2-dev:amd64 2.0.10+dfsg1-3 amd64 Simple DirectMedia Layer development files
- libsdl2-image-dev:amd64 2.0.5+dfsg1-2 amd64 Image loading library for Simple DirectMedia Layer 2, development files
- libsdl2-ttf-dev:amd64 2.0.15+dfsg1-1 amd64 TrueType Font library for Simple DirectMedia Layer 2, development files

## 使用方法

- ゲームや遺伝的アルゴリズムの設定は __property.json__ に記述
- 結果のファイル出力はコマンドラインの第二引数にファイル名を記述することで取得可能(json形式で出力される)
- 初期値は第一世代の0番目の要素以外はランダム。

## property.jsonの詳細

|属性|説明|
|:--|:--|
|init_value|第1世代の1つ目の要素の値を指定|

### genetic_algorithm_property

|属性|説明|
|:--|:--|
|crossover_rate|交叉する確率|
|mutation_rate|突然変異する確率|
|num_array_elements|1世代あたりの要素数|
|num_generation|世代数|
|is_selected_elite|世代交代時にエリートを残すかどうか|
|num_selected_elite|世代交代時にエリートを残す場合の、エリート数|
|ranking_probability|ランキング選択時のランク毎の選択確率|
|tournament_size|トーナメント選択時のトーナメントサイズ|
|num_multi_point|多点交叉時の交叉点の数|
|crossing_tag|交叉方法を示すタグ<br>0: 1点交叉<br>1: 2点交叉<br>2: 多点交叉<br>3: 一様交叉|
|selection_tag|選択方法を示すタグ<br>0: ルーレット選択<br>1: ランキング選択<br>2: トーナメント選択|

### game_property
    
|属性|説明|
|:--|:--|
|npc_search_depth|npcの探索深度<br>最悪の場合の計算量はO(160^n)|
|num_max_used_block|使用ブロック数の上限|
|used_block_score_multiple|使用したブロック1つあたりに対するスコア倍率|
|tetris_score_multiple|4ライン同時消しに対するスコア倍率|
|three_lines_score_multiple|3ライン同時消しに対するスコア倍率|
|two_lines_score_multiple|2ライン同時消しに対するスコア倍率|
|one_line_score_multiple|1ライン消しに対するスコア倍率|

## 実行結果(gif)

- 初期値
    
    ![\[1-1-1-1-1\]](https://github.com/leon0128/GeneticAlgorithm/blob/media/ga_gen1(1-1-1-1-1).gif)

- 要素数30、エリート数2、一様交叉、トーナメント選択(大きさ3)、交叉確率0.6、突然変異確率0.1の時、13世代目の最高評価

    ![\[19-65-18-0-60\]](https://github.com/leon0128/GeneticAlgorithm/blob/media/ga_gen13(19-65-18-0-60).gif)

- 自分で設定した値

    ![\[10-49-5-1-99\]](https://github.com/leon0128/GeneticAlgorithm/blob/media/ga_self(10-49-5-1-99).gif)

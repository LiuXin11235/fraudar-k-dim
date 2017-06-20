# Fraudar k-dim

> proposed by 毕克



- 运行方式

  ```shell
  #在目录下放置event.csv
  g++-7 f.cpp -O2 -o f
  ./f < no.txt 
  ```

- 文件说明

  - no.txt  将event.csv中的所有数据表示为一个整数，可按照行号查阅event.csv，得到原始数据
  - wd.txt 暂时无用

- 最可疑的输出结果

  ```
  rounds: 7
  Deleted Edges 56327
  bestFraction: 22.853230
  bestNumerator: 631549.000000
  bestDenominator: 27635.000000
  Different users[0]: 5
  Different users[1]: 6830
  Different users[2]: 376
  Different users[3]: 28
  Different users[4]: 1
  Different users[5]: 14
  Different users[6]: 127
  Different users[7]: 2
  Different users[8]: 213
  Different users[9]: 6
  ```
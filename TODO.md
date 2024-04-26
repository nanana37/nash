

functions
- execve
  - execvpでの仮実装
  - PATH追跡する！
- cd
  - 完成
- pwd
  - 完成
- exit
  - 大体完成
  - チャイルドを全部killする処理を入れたい（kill(0, SIGKILL)はKilledと表示されて強制終了する
- redirect
  - command_tでredirect先を管理
- pipe
  - command_tでpipe先を管理
- ^C
  - できた？
  - 子プロセスが勝手に終了してくれるのがよくわからない、ハンドラが共有されていない？
  - execveはプロセスのメモリ内容をすり替える！！！呼び出し側で変えられないってこと？
- ^Z
  - 上と同様の疑問あり。おそらく解決
  - fgであるはずのchild_pidをjobsに追加しているが、確証はあるのか？
  - jobsの名前をコマンドの名前にしたい（command_t終わってから）
- bg
  - 引数でjobsの番号を指定できるようにしたが、セグフォが発生する
- fg
  - できてない
  - 制御端末をfgにしたいプロセスと一致させる！
* error

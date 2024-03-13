# ICFL2SA

Alcune cose state implementate ma ho deciso di non pubblicarle qui, tipo tutte quelle scritto nel `.gitignore`.

Ad esempio i risultati ottenuti con Valgrid e le analisi ottenibili da chiunque tramite comando.

È importante creare un file `input_file.txt` dove andrà inserita la stringa da immettere come input.

una volta inserita bisogna compilare ed eseguire. Non ho usato ottimizzazioni di compilazione ma se volete farlo, potete modificare il comando liberamente.

```
g++ *.cpp -o main
./main
```

Per esegire il **testing** invece:

```
valgrind --leak-check=full--show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./main
```

Per creare un file di input tramite linea di comando è stato implementato lo script `python_string_generator.py`.
Per lanciarlo basta lanciare il comando qui sotto e creerà il file in modalità di **scrittura** il file `input_file.txt`

```
python3 python_string_generator.py N
es.
python3 python_string_generator.py 1000000
```
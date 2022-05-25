# mk2fg dns converter README

## Configurazione file mk-dns.conf

Copiare righe formato `add address=ip name=string` all'interno del file lasciando come ultima riga una riga vuota

Esempio:

```conf
add address=192.168.122.1 name=router.domain1.com
add address=192.168.172.3 name=name1.domain2.com
add address=192.168.122.119 name=name2.domain1.com

```

## Compilazione configuratore LINUX

Compilare il programma con comando `gcc mk2fg-dns.c -o ./mk2fg-dns-converter/mk2fg-dns`

## Esecuzione configuratore LINUX

Posizionarsi sulla cartella di esecuzione `cd mk2fg-dns-converter`

Lanciare il programma con comando `./mk2fg-dns`

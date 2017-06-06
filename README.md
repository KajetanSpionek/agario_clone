# README #

Nazwa projektu: agario_clone

Autorzy: Kajetan Śpionek, Wojciech Przybysz

### Przygodowanie środowiska ###

1)Instalacja biblioteki boost: 
 Ubuntu/Debian: sudo apt-get install libboost-all-dev

2)Instalacja cppunit:
 sudo apt-get install libcppunit-dev

3)By uruchomić bota należy użyć przeglądarki z udostępnionym dostępem do plików:
 google-chrome --allow-file-access-from-files

### Gra ###

Program budowany jest przy użyciu sCons. 
Aby uruchomić serwer, należy urochomić zbudowany w katalogu 'server' program komendą:
./program 0.0.0.0 7777

Uruchomienie klienta:
- uruchomić przeglądarkę z opcją dostępu do plików, np.
	google-chrome --allow-file-access-from-files
- w katalogu client, uruchomić w przeglądarce index.html,
- wybrać nick, zatwierdzić i grać.

By ułatwić testowanie stworzono bot'a gry.
Uruchomienie bota:
- - w katalogu bot, uruchomić w przeglądarce index.html,
- wybrać nick, zatwierdzić.

## Тестовый проект 
### Окружение
* **Fedora25**. Уже стояла на ПК, насколько я знаю, CentOS это родственная ОС, поэтому решил делать в Fedora.
* **Qt 5.3.2, GCC (x86 32bit)**
* **SQLite**. Я не осилил собрать плагин для QT ни для PostgreSQL, ни для Oracle. 
Мой qmake ведет себя не так, как того требует инструкция по установке. 
Разбираться подробно не стал - специфика работы с разными серверами БД по большей части скрыта внутри QSQLDatabase и разница в коде, думаю, мала. Таблицу в базе данных создал такую:
```
  CREATE TABLE "testdata" (
      "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
      "text" TEXT NOT NULL,
      "value" INTEGER NOT NULL
  )
```

### Как проверить
Скачайте с помощью git clone или загрузите вручную исходные файлы в одну папку, например `home/test/`. Соберите исполняемый файл сервиса test (или воспользуейтесь готовым из той же папки)
- **Запустите сервис test** с аргументами [port] [БД-файл] [лог-файл], например 
```
  cd /home/test/
  ./test 1234 test_db log.txt
```
  Приходится писать `./test`, вместо `test`, т.к. это оказалось неудачное имя для сервиса, он пересекается со стандартной системной утилитой.
  
  Закройте терминал, чтобы проверять работу в фоновом режиме (у нас же сервис).
- **Запустите клиент**. Я реализовал в сервисе примитивное REST API для клиент-серверного обмена, поэтому в качестве клиента предлагается просто воспользоваться браузером. 
  - **Добавление записи в БД**. Для добавления записей используется POST-запрос. Откройте html-форму "REST Client.html" (тоже лежит в корне), заполните и отправьте. Браузер покажет newID. Неудобный нюанс - в этой форме порт и имя хоста захардкодены на localhost:1234, поэтому желательно запускать сервис локально на порте 1234, как и предлагается выше. 
  Создайте несколько новых записей.
<<<<<<< HEAD
  - **Запрос записи из БД**. Используется GET-запрос. В адресной строке наберите `http://localhost:1234/id/1` чтобы запросить из БД запись с id = 1. Сервис возвращает запись в виде  JSON-объекта в теле HTTP- ответа.
Попробуйте ввести другой путь или формат в адресной строке: текстовым ответом сервис оповещает об ошибках в использовании его REST API.
- **Остновка сервиса**. Реализована через команду REST от клиента. Наберите в адресной строке браузера `http://localhost:1234/stop`. 
- **Откройте лог**. В логе нет сообщений, выдаваемых клиенту при работе (в том числе об ошибках с работой REST API или сбоев при работе с таблицей БД), только записи о запуске, остановке и подключению к БД.
— проверка
- проверка 2
— добавление в предыдущий коммит конфликтующее внутри строки
— правка после добавления файла new.txt

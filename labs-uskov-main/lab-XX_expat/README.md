# Необязательная лабораторная работа

## Подготовка

Для выполнения задания потребуется библиотека для парсинга XML -- SAX Parser "Expat".
```
sudo apt-get install libexpat1-dev
```
Также рекомендуется поставить утилиты для работы с xml.
```
sudo apt-get install libxml2-utils
```
При помощи xmllint из этого пакета можно проверить, что xml корректный.

Пример работы с expat в файле `expat_example.c`
Линковать с ключем `-lexpat`
```
gcc expat_example -lexpat
```

## Задание

Реализовать структуру данных и функции для работы с записной книжкой.
В записной книжке про каждого человека хранится Фамилия Имя Отчество и несколько 
его телефонов. Записная книжка хранится в XML файле.

### Формат XML файла
```
<?xml version="1.0" encoding="UTF-8"?>
<phonebook>
    <human name="Ivan Ivanonich Ivanov">
        <phone>1234567</phone>
        <phone>9211234567</phone>
        <phone>9111234567</phone>
    </human>
    <human name="Petr Petrovich Petrov">
        <phone>7654321</phone>
        <phone>9051234567</phone>
    </human>
</phonebook>
```

Имя, отчество, фамилия должны хранится в отдельных полях. 
Можно считать, что длина каждой из этих строк не превышает 255 символов
(используются только английские буквы). Телефоны должны храниться в виде 
двумерного массива строк. Можно считать, что длина телефона не превышает 
20 символов, и у каждого человека не более 10 телефонов. 
Для разбора поля name из XML в поля структуры должна использоваться функция `strtok`.

```
struct human_s {
    char name[256];
    char middle_name[256];
    char family_name[256];
    char phones[10][21];
};
```

Количество записей в XML файле заранее неизвестно. 
* Структуры `human_s` должны храниться в виде динамического массива, для выделения
дополнительного места должна использоваться функция `realloc`. 
При перевыделении массива разумно увеличивать его размер сразу в 2 раза.
* После загрузки все записи о людях хранятся отсортированными в алфавитной порядке 
по полю `family_name`. Для сортировки необходимо воспользоваться стандартной функцией `qsort`.
* Функции работы с диском в случае ошибки возвращают ненулевое число.
* Функция `gen_phonebook` очищает записную книжку и наполняет ее случайным содержимым. 
Для генерации случайных имен можно воспользоваться именами из `names.h`

```
struct phonebook_s {
    human_t *humans;
    size_t size;
    size_t capacity;
};

int load_phonebook_xml(const char *filename, phonebook_t *book);
int save_phonebook_xml(const char *filename, phonebook_t *book);
void print_phonebook(phonebook_t *book);
void gen_phonebook(phonebook_t *book, size_t size);
void clear_phonebook(phonebook_t *book);
human_t get_human(phonebook_t *book, char* family_name);

// служебная функция
void push_back_human(phonebook_t *book, human_t *human);
```

## Консольное приложение

Реализовать программу, которая загружает записную книжку из xml файла, выводит ее на экран. 
Затем генерирует новую случайную книжку и сохраняет ее в xml файл.

### Интерфейс
```
./main <infile> <outfile>
```

* `<infile>` - имя входного xml файла
* `<outfile>` - имя выходного xml файла

Формат сгенерированного xml должен соответствовать [формату xml-файла](#формат-XML-файла)

### Формат для вывода книжки на экран.

На первой строке размер книжки. Далее перечисление человек и их телефонов в формате представленном ниже. 
Люди отделяются пустой строкой.

```
<book size>
name: <name of human[0]>
middle name: <middle name of human[0]>
family name: <family name of human[0]>
phone: <first phone of human[0]>
phone: <second phone of human[0]>...

name: <name of human[1]>
...
```

Пример вывода для pbook.xml
```
2
name: Ivan
middle name: Ivanonich
family name: Ivanov
phone: 1234567
phone: 9211234567
phone: 9111234567

name: Petr
middle name: Petrovich
family name: Petrov
phone: 7654321
phone: 9051234567
```

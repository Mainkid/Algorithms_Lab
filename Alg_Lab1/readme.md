# Hammurabi
___
# Описание программы:
Игра выполнена в структурной парадигме программирования.
Все игровые параметры (границы цен на акр земли, вероятность чумы и прочее) объявлены как константы.
Сохранение игрового процесса осуществляется автоматически, в конце каждого раунда.
#### Основные функции:
- print_game - выводит информацию игроку в конце каждого раунда;
- ask_player - запрашивает у игрока информацию о продаже/покупке земли, акрах земли для возделывания и прочем;
- check_inputed_values - проверяет введенную пользователем информацию;
- show_game_results - выводит результаты игры, подсчитывает очки;
- load_data - загрузка сохранения из файла;
- save_data - сохранение прогресса в файл.
___
#### Алгоритм:
Игра начинается со следующими параметрами:
- В городе проживает 100 человек
- В закромах города 2800 бушелей пшеницы
- Город занимает 1000 акров земли
В начале каждого раунда игрок видит
- Номер раунда
- Количество людей, умерших от голода (если не было, то не показывается)
- Количество людей, приехавших в город (если не было, то не показывается)
- Была ли чума
- Текущее население города
- Сколько всего пшеницы было собрано и сколько пшеницы было получено с акра
- Сколько пшеницы уничтожили крысы
- Сколько акров сейчас занимает город
- Какова цена одного акра земли в этом году
Цена акра определяется случайно в начале каждого раунда из диапазона от 17 до 26

Затем игрок вводит:
- Количество акров земли, которые он хочет купить
- Количество акров земли, которые он хочет продать
- Количество бушелей пшеницы, которые будут использованы в качестве еды
- Количество акров земли, которые необходимо засеять пшеницей

Игра контролирует вводимые значения, и, если значения оказываются некорректными (например, количество земли, которое хочет купить игрок не может быть оплачено имеющимся пшеницей, или введено отрицательное количество акров земли), выводит соответствующие предупреждения.

Для контроля используются следующие значения (некоторые из них скрыты от игрока, он должен определить их сам):

- Продажа и покупка земли выполняются по текущей цене
- Каждый житель потребляет 20 бушелей пшеницы в год
- Каждый житель может обрабатывать максимум 10 акров земли
- Каждый акр требует .5 бушелей пшеницы на семена

Переход к следующему раунду определяется правилами:

- Количество пшеницы, собранного с акра определяется случайно из диапазона от 1 до 6
- Крысы съедают случайное количество пшеницы из диапазона от 0 до 0.07*имеющееся количество бушелей с учетом сборов
- Каждый житель потребляет 20 бушелей пшеницы в раунд, те, кому не хватает еды, умирают от голода
- Если за раунд умерли от голода более 45% населения, игрок сразу же проигрывает
- Количество вновь прибывших определяется по формуле: умерших_от_голода /2 + (5 – сбор_с_акра) * бушелей_пшеницы / 600 + 1 затем, если это количество меньше 0, берется 0, если больше 50, то 50
- Чума возникает с вероятностью 15%, если это происходит, то население уменьшается наполовину с округлением вниз

После 10 раунда игра вычисляет следующие статистики:

Среднегодовой процент умерших от голода P
Количество акров земли на одного жителя L и выставляет игроку оценку по результатам правления:
- Если P > 33% и L < 7 – плохо («Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города. Теперь вы вынуждены влачить жалкое существование в изгнании»)
- Если P > 10% и L < 9 – удовлетворительно («Вы правили железной рукой, подобно Нерону и Ивану Грозному. Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем»)
- Если P > 3% и L < 10 – хорошо («Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова»)
- В противном случае – отлично («Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше»)

___
### Пример игры:
```
Мы обнаружили сохранение вашей прошлой игры. Хотите ли вы продолжить ее?

1. Да
2. Нет
1
Мой повелитель, соизволь поведать тебе
        в году 6 твоего высочайшего правления
         2 человек умерло с голоду, и 2 человек прибыли в наш великий город
         Чума уничтожила половину населения
         Население города сейчас составляет 67 человек
         Мы собрали 5500 бушелей пшеницы, по 5 бушеля с акра
         Крысы истребили 515 бушелей пшеницы, остаив 9433 в амбарах
         Город сейчас занимает 1100 акров
         1 акр земли сейчас стоит 19 бушель

Что пожелаешь, повелитель?
        Сколько акров земли повеливаешь купить? 0
        Сколько акров земли повеливаешь продать? 0
        Сколько бушелей пшеницы повелеваешь съесть? 1300
        Сколько акров земли повелеваешь засеять? 600

Мой повелитель, соизволь поведать тебе
        в году 7 твоего высочайшего правления
         2 человек умерло с голоду, и 18 человек прибыли в наш великий город
         Население города сейчас составляет 83 человек
         Мы собрали 2400 бушелей пшеницы, по 4 бушеля с акра
         Крысы истребили 286 бушелей пшеницы, остаив 9947 в амбарах
         Город сейчас занимает 1100 акров
         1 акр земли сейчас стоит 17 бушель

Что пожелаешь, повелитель?
        Сколько акров земли повеливаешь купить? 20
        Сколько акров земли повеливаешь продать? 0
        Сколько бушелей пшеницы повелеваешь съесть? 1600
        Сколько акров земли повелеваешь засеять? 800

Мой повелитель, соизволь поведать тебе
        в году 8 твоего высочайшего правления
         3 человек умерло с голоду, и 33 человек прибыли в наш великий город
         Население города сейчас составляет 113 человек
         Мы собрали 2400 бушелей пшеницы, по 3 бушеля с акра
         Крысы истребили 462 бушелей пшеницы, остаив 9545 в амбарах
         Город сейчас занимает 1120 акров
         1 акр земли сейчас стоит 17 бушель

Что пожелаешь, повелитель?
        Сколько акров земли повеливаешь купить? 200
        Сколько акров земли повеливаешь продать? 0
        Сколько бушелей пшеницы повелеваешь съесть? 2400
        Сколько акров земли повелеваешь засеять? 1100

Мой повелитель, соизволь поведать тебе
        в году 9 твоего высочайшего правления
         0 человек умерло с голоду, и 21 человек прибыли в наш великий город
         Население города сейчас составляет 134 человек
         Мы собрали 3300 бушелей пшеницы, по 3 бушеля с акра
         Крысы истребили 318 бушелей пшеницы, остаив 6177 в амбарах
         Город сейчас занимает 1320 акров
         1 акр земли сейчас стоит 17 бушель

Что пожелаешь, повелитель?
        Сколько акров земли повеливаешь купить? 0
        Сколько акров земли повеливаешь продать? 0
        Сколько бушелей пшеницы повелеваешь съесть? 3000
        Сколько акров земли повелеваешь засеять? 1320

Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова
P: 0.0346697 L: 9
```
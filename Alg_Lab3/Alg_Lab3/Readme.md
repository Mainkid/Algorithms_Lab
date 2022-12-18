# QuickSort
___
# Задача:

 Задание
В вашу задачу входит разработка обобщенной функции sort(),
реализующей алгоритм быстрой сортировки со следующими
оптимизациями:
- Выбор опорного элемента, как медианы из первого, среднего и
последнего элемента сортируемого интервала;
- Исключение хвостовой рекурсии: функция должна определять длины
получившихся интервалов после разбиения и рекурсивно
сортировать интервал меньшей длины, итеративно обрабатывая
интервал большей длины;
- Использование алгоритма сортировки вставками для коротких
интервалов (длину такого интервала необходимо подобрать
экспериментально).
- Использовать move-семантику для обмена элементов в процессе
разбиения и при сортировке вставками.
Функция должна принимать три аргумента: начало first и конец last
сортируемого интервала и предикат сортировки comp, который принимает
два аргумента – элементы массива a и b, и возвращает true, если a < b, то
есть в отсортированном массиве элемент a должен располагаться перед b.



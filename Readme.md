Библиотека с реализацией различных агоритмов по заделыванию отверстий в 3д объектах  
файловая структура:  

`/base` - основные структуры, используещиеся для хранения и манипуляций с мешом  
`/examples` - примеры применения библиотеки - можно менять файлы и запускать свои эксперименты с различными моделями  
`/experiments` - папка где хранятся все параметры различных экспериментов - применяемые алгоритмы, входные и выходные модели  
`/filling_strategy` - главная часть - здесь собраны все части из которых можно собрать стратегию заделывания отверстий   
`/models` - различные модели на которых можно производить эксперименты  

для удобства запуска и экспериментирования - `все параметры надо менять в коде и после комплировать заново` - рекомендуется использовать IDE, например, CLion

в папке `filling_strategy` в файлах `.hpp` для всех классов имеется описание что они делают
чтобы при наведении на имя класса эта документация всплывала по подсказке рекомендуется использовать IDE, например, CLion



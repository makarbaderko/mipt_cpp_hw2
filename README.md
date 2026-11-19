## 

Задание номер 2 по курсу С++ 26/27. Выполнил студент группы Б05-621 Бадерко Макар Александрович.

## Сборка и запуск

Из каталога `Fleet`:

```sh
cmake -S . -B build
cmake --build build
./build/fizkat
```

В VS Code откройте каталог `Fleet`. На macOS `Cmd+Shift+B` собирает проект,
а `Tasks: Run Task` → `ФизКат: запуск` собирает и запускает его.

Демонстрация: `./build/fizkat < examples/demo.in`.
Ожидаемый вывод: [demo.out](examples/demo.out).

## Наследование

```text
InventoryItem (абстрактный)
`-- NamedItem
    |-- Transport
    |   |-- ElectricScooter + EnergyConsumer
    |   |-- Bicycle
    |   |-- ElectricBicycle + EnergyConsumer
    |   `-- Skateboard
    |-- Helmet
    `-- DockStation + EnergyConsumer

EnergyConsumer (абстрактный интерфейс)
```

Все типы инвентаря описаны в [inventory.h](fizkat/inventory.h).
Общие поля и печать реализованы в `NamedItem`. Знак `+` обозначает множественное наследование.
Ромба нет, виртуальное наследование не требуется. В [Fleet](fizkat/fleet.cpp)
печать, подсчёт энергии и отбор новичков вызывают виртуальные методы через базис `InventoryItem`.

## Smart-указатели

- `unique_ptr<InventoryItem>` создаётся в [main.cpp](main.cpp)
  и владеет новой позицией до техосмотра. При отказе объект уничтожается, парк не меняется.
- После приёмки [Fleet::Add](fizkat/fleet.cpp) передаёт объект из `unique_ptr`
  в `shared_ptr<InventoryItem>`. Парк хранит принятые позиции в реестре с `vector<shared_ptr<InventoryItem>>`.
- `weak_ptr<InventoryItem>` в `Fleet::focus_` наблюдает выбранную позицию и не владеет ею.
  После `remove` объект уничтожается, а `show_focus` печатает `expired`.
  Повторное добавление того же номера фокус не восстанавливает.

## Исключения

В [errors.h](fizkat/errors.h) общий базис `FleetError` наследует `std::runtime_error`:

- `InspectionError` — имя транспорта содержит `broken` без учёта регистра;
- `CommandError` — неверная команда, аргументы, дубликат или отсутствующий номер.

Техосмотр выполняется методом `Transport::Inspect()` до добавления в парк. Для вещей он не нужен.
При ошибке парк не меняется; `error: ...` печатается только в `catch` в [main.cpp](main.cpp).

## Операторы и шаблоны

В [inventory.cpp](fizkat/inventory.cpp): `operator<<` печатает позицию, `operator<`
сортирует по номеру, дополнительный `operator==` сравнивает номера при проверке дубликатов.

В [registry.h](fizkat/registry.h) шаблон `Registry<T>` хранит позиции парка
как `Registry<shared_ptr<InventoryItem>>`. Шаблонная функция `SumEnergy` вызывается из `Fleet::TotalEnergy()`.

## Дополнительная часть

Добавлены `Skateboard` и команда `add skateboard id name score`, отчёт `count_novice`
и команда `report`, печатающая число позиций и суммарную энергию.
`novice` и `count_novice` работают без аргументов. Порог 6 задан константой
`kMinimumNoviceScore` в [inventory.cpp](fizkat/inventory.cpp).
Позиции печатаются как `id name`, энергия — с одним знаком после точки.

## Использование ИИ

ИИ использовался для более быстрого взаимодействия с документацией языка C++ и поиска ответов на вопросы при возникновении сложностей при выполнении задания. Также он помог с оформлением `README`.
# SolarHaven-Public

### EN:  
This repository is part of the codebase for the Solar Haven project, which is currently under development.
It is intended to demonstrate my approach to writing code and using the Unreal Engine toolset.

The codebase itself is located in the Content folder.

The code is divided into several modules:
**SHCore**, **SHSubsystems**, **SHUI**, **SHComponents**, and **SolarHaven**.

1. **SHCore**  
This module contains base classes, structures, interfaces, and utility functions.
2. **SHSubsystems**  
This module contains Subsystem classes that implement global game systems, such as the in-game time system.
3. **SHUI**  
This module contains code related to the user interface. It includes classes and logic responsible for displaying information to the player and handling player interaction with the UI.
4. **SHComponents**  
This module contains Actor Components that can be attached to different Actors. For example, the inventory system can be attached to both the player character and a chest.
5. **SolarHaven**  
The main gameplay module. It contains gameplay logic related to specific game mechanics and objects.

**NOTE**:  
The GridSystem is currently unfinished and still contains some bugs.
---
### RU:
Данный репозиторией является частью кодовой базы проекта Solar Haven, который сейчас разрабатывается.
Он необходим, чтобы продемонстрировать мой подход к написанию кода и использованию инструментария Unreal Engine. 

Сама кодовая база лежит в папке Content.  
Код разделен на модули: **SHCore**, **SHSubsystems**, **SHUI**, **SHComponents**, **SolarHaven**
1. **SHCore**  
В данном модуле хранятся базовые классы, структуры, интерфейсы и вспомогательные функции. 
2. **SHSubsystems**  
В данном модуле находятся Subsystem-классы, реализующие глобальные системы игры (например, система внутриигрового времени)
3. **SHUI**  
В данном модуле находится код, связанный с пользовательским интерфейсом. Здесь располагаются классы и логика, отвечающие за отображение информации игроку и взаимодействие игрока с UI
4. **SHComponents**  
В данном модуле находятся Actor Components, которые можно подключать к различным Actor. Например, система инвентаря, которую можно подключить как к персонажу игрока, так и к сундуку, например. 
5. **SolarHaven**  
Основной игровой модуль. Здесь хранится игровая логика, связанная с конкретными механиками и объектами игры.

**ПРИМЕЧАНИЕ**:  
На данный момент GridSystem ещё не доделана, в ней есть баги. 
# Deema CLI

### CLI for [dc-api-core](https://github.com/dimacrafter/dc-api-core)

## Установка / Install


### macOS

Если он был уже установлен, то удалите прошлую версию: `rm /usr/local/bin/deema`

```bash
curl -o /usr/local/bin/deema https://files.deema.help/bin/deema-mac
chmod +x /usr/local/bin/deema
```

### Linux

Если он был уже установлен, то удалите прошлую версию: `rm /usr/local/bin/deema`

```bash
curl -o /usr/local/bin/deema https://files.deema.help/bin/deema-linux
chmod +x /usr/local/bin/deema
```

### Windows

Нет скомпилированной версии, но возможно можно собрать.

## Сборка / Build

Необходимые библиотеки можно установить с помощью [conan](https://conan.io/downloads) командой `conan install .`.

Для сборки на Unix-like ОС используйте `build.sh`.

## Использование / Usage

Просто выполните команду `deema`, чтобы ознакомиться с краткой исторической справкой.

## TODO

- [ ] make/m из пресетов на files.deema.help
- [ ] integrate/i angular/ng (с автоустановкой и настройкой `dc-api-client`)
- [ ] start для быстрого запуска бекенда в фоне, для продакшна
- [ ] Более приятный вывод CLI
# <a id="Top">Tuya WaterLeak Sensor Zigbee with custom firmware</a>

### Custom firmware for Tuya sensor models

- _TZ3000_k4ej3ww2

<img src="https://raw.githubusercontent.com/slacky1965/ts0207_tz3000_zed/refs/heads/main/doc/images/ts0207_tz3000.jpg"/>

<img src="https://raw.githubusercontent.com/slacky1965/ts0207_tz3000_zed/refs/heads/main/doc/images/board.jpg"/>

<img src="https://raw.githubusercontent.com/slacky1965/ts0207_tz3000_zed/refs/heads/main/doc/images/board1.jpg"/>

**Автор не несет никакой отвественности, если вы, воспользовавшись этим проектом, превратите свой умный датчик в полоумный.**

Если у вас другая сигнатура, лучше не заливать, не проверив на совпадение GPIO.

Проверялся только в zigbee2mqtt.

## Зачем. 


## Что получилось. 

**About**

<img src="https://raw.githubusercontent.com/slacky1965/ts0207_tz3000_zed/refs/heads/main/doc/images/about.jpg"/>

**Exposes**

<img src="https://raw.githubusercontent.com/slacky1965/ts0207_tz3000_zed/refs/heads/main/doc/images/exposes.jpg"/>

**Reporting**

<img src="https://raw.githubusercontent.com/slacky1965/ts0207_tz3000_zed/refs/heads/main/doc/images/reporting.jpg"/>

## Как обновить.

Сначала подключаем к z2m один внешний [конвертор](https://github.com/slacky1965/ts0207_tz3000_zed/tree/main/zigbee2mqtt) `zg_222za_orig.js`. Он активирует OTA в z2m для датчика с прошивкой от Tuya.

Далее нужно добавить локальное хранилище обновлений. 

Создаем директорию `images` в директории z2m и кладем туда файл [1141-d3a3-1111114b-ts0207_tz3000_zrd.zigbee](https://github.com/slacky1965/ts0207_tz3000_zed/raw/refs/heads/main/bin/1141-d3a3-1111114b-ts0207_tz3000_zrd.zigbee).

Копируем в директорию z2m файл [local_ota_index.json](https://github.com/slacky1965/ts0207_tz3000_zed/blob/main/zigbee2mqtt/local_ota_index.json)

```
ota:
  zigbee_ota_override_index_location: local_ota_index.json
```

А конвертор `zg_222za_orig.js` кладем в директорию `external_converters`, которую нужно создать в корне z2m.

Далее перегружаем z2m. И видим у нас новое устройство.

<img src="https://raw.githubusercontent.com/slacky1965/ts0207_tz3000_zed/refs/heads/main/doc/images/tuya_ready.jpg"/>

Далее идем в раздел OTA. И видим там свое устройство. Жмем проверить обновления.

<img src="https://raw.githubusercontent.com/slacky1965/ts0207_tz3000_zed/refs/heads/main/doc/images/update.jpg"/>
	
Жмем на появившуюся красную кнопку. И обновляемся.

Если все не так, как описано, значит вы что-то сделали не по инструкции (не положили файл куда нужно, не перегрузили z2m) или сигнатуры вашего датчика нет в списке поддерживаемых устройств.

> [!WARNING]
> Внимание!!! Если в процессе вы обнаружите на каких-то устройствах Туя, которые возможно у вас есть еще в системе, новое обновление, то обновлять ничего не нужно!!! Иначе вы зальете в это устройство прошивку от датчика и получите кирпич!!! Если же процесс обновления по ошибке уже начался, то просто обесточьте это устройство!!!

Далее ждем окончания.

После обновления нужно удалить устройство из z2m. Перегрузить z2m. Разрешить сопряжение. Зажать кнопку на 5 секунд, пока светодиод не начнет моргать и отпустить. Начнется сопряжение.

<img src="https://raw.githubusercontent.com/slacky1965/ts0207_tz3000_zed/refs/heads/main/doc/images/joined.jpg"/>

Все, датчик готов к работе.

> [!WARNING]
> Внимание!!! Залитая первый раз прошивка является промежуточной. В ней есть код для проверки `bootloadr'а`, который несет потенциальную угрозу порчи этого `bootloader'a` в будущем. Поэтому нужно обновиться еще раз уже на окончательную версию, где этот код не используется. Сразу сделать без этой проверки нельзя (привет Tuya). Для понимания - промежуточная версия имеет номер 1.0.00.

## Параметры устройства.


## Немного про настройку прямого биндинга для управления каким-либо устройством.

Для управление внешним устройством напрямую нужно сперва настроить биндинг. Это делается во вкладке "Bind" z2m. Выбираем Эндпоинт 1, затем выбираем устройство, которым хотим управлять, отмечаем кластер `OnOff`. Затем нажимаем кнопку на самом датчике, чтобы его разбудить и после жмем "Bind" в интерфейсе z2m. Система оповестит об успешном соединении или об ошибке.

<img src="https://raw.githubusercontent.com/slacky1965/ts0207_tz3000_zed/refs/heads/main/doc/images/binding.jpg"/>

## Потребление

<img src="https://raw.githubusercontent.com/slacky1965/ts0207_tz3000_zed/refs/heads/main/doc/images/read_period_10sec.jpg"/>

<img src="https://raw.githubusercontent.com/slacky1965/ts0207_tz3000_zed/refs/heads/main/doc/images/read_period_60sec.jpg"/>

Связаться со мной можно в **[Telegram](https://t.me/slacky1965)**.

### Если захотите отблагодарить автора, то это можно сделать через [ЮMoney](https://yoomoney.ru/to/4100118300223495)

P.S. Датчик покупался [вот тут](https://aliexpress.ru/item/1005007627377380.html?sku_id=12000041560940091).

## История версий
- 1.0.01
	- Тестовая версия. Команда OnOff отправляется не только при обнаружении протечки, но и при устранении протечки. Сделано для более легкого тестирования.

[Наверх](#Top)



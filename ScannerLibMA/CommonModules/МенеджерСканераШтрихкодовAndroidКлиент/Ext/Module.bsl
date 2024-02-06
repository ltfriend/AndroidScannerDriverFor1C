﻿///////////////////////////////////////////////////////////////////////////////////////////////////////
// (с) Tolkachev Pavel, 2020
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////////////////////////////

#Область ПрограммныйИнтерфейс

Функция ЕстьНеобработанноеСобытие() Экспорт
	
	Возврат Не глПараметрыСканераШтрихкодовAndroid.СобытиеОбработано;
	
КонецФункции 

Процедура НачатьУстановкуДрайвера(ОповещениеОЗавершении = Неопределено) Экспорт
	
	НачатьУстановкуВнешнейКомпоненты(ОповещениеОЗавершении, МестоположениеДрайвера());
	
КонецПроцедуры 

Процедура НачатьПодключениеДрайвера(ОповещениеОЗавершении = Неопределено) Экспорт
	
	НачатьПодключениеВнешнейКомпоненты(
		ОповещениеОЗавершении,
		МестоположениеДрайвера(),
		"com_ptolkachev_AndroidScannerExtension",
		ТипВнешнейКомпоненты.Native);
	
КонецПроцедуры 

Процедура ОбработатьСобытие(Источник, Событие, Данные) Экспорт
	
	Если Источник = "com_ptolkachev_AndroidScanner" И Событие = "Штрихкод" Тогда
		
		ИмяСобытия = "ScanData";
		
		Штрихкод = СокрЛП(Данные);
		
		МассивВторогоПорядка = Новый Массив;
		МассивВторогоПорядка.Добавить(Данные);
		МассивВторогоПорядка.Добавить(Штрихкод);
		МассивВторогоПорядка.Добавить(0);
		
		МассивМассивов = Новый Массив;
		МассивМассивов.Добавить(Штрихкод);
		МассивМассивов.Добавить(МассивВторогоПорядка);
		
		Оповестить("ScanData", МассивМассивов, "ПодключаемоеОборудование");
		
	КонецЕсли; 
	
КонецПроцедуры 

Процедура Отключить(ИдентификаторКлиента) Экспорт
	
	Если глПараметрыСканераШтрихкодовAndroid.Сканер = Неопределено Тогда
		Возврат;
	КонецЕсли; 
	
	Клиент = глПараметрыСканераШтрихкодовAndroid.Сканер.Клиенты.Найти(ИдентификаторКлиента);
	Если Клиент = Неопределено Тогда
		Возврат;
	КонецЕсли; 
	
	КоличествоПодключенных = глПараметрыСканераШтрихкодовAndroid.Сканер.КоличествоПодключенных;
	
	Если КоличествоПодключенных = 1 Тогда
		Оповещение = Новый ОписаниеОповещения("ОтключениеЗавершение", ЭтотОбъект);
		глПараметрыСканераШтрихкодовAndroid.Сканер.ОбъектДрайвера.НачатьВызовОтключить(Оповещение, "");
	Иначе
		глПараметрыСканераШтрихкодовAndroid.Сканер.Клиенты.Удалить(Клиент);
		глПараметрыСканераШтрихкодовAndroid.Сканер.КоличествоПодключенных = КоличествоПодключенных - 1;
	КонецЕсли;
	
КонецПроцедуры 

Процедура Подключить(ИдентификаторКлиента) Экспорт
	
	ОчиститьСообщения();
	
	Если Не глПараметрыСканераШтрихкодовAndroid.УстройствоИспользуется Тогда
		Возврат;
	КонецЕсли; 
	
	Если глПараметрыСканераШтрихкодовAndroid.Сканер = Неопределено Тогда
		Оповещение = Новый ОписаниеОповещения("ПодключениеЗавершение", ЭтотОбъект, ИдентификаторКлиента);
		НачатьПолучениеОбъектаДрайвера(Оповещение);
	Иначе
		глПараметрыСканераШтрихкодовAndroid.Сканер.Клиенты.Добавить(ИдентификаторКлиента);
		
		КоличествоПодключенных = глПараметрыСканераШтрихкодовAndroid.Сканер.КоличествоПодключенных;
		глПараметрыСканераШтрихкодовAndroid.Сканер.КоличествоПодключенных = КоличествоПодключенных + 1;
	КонецЕсли; 
	
КонецПроцедуры 

Функция ПреобразоватьДанныеСоСканераВМассив(Параметр) Экспорт
	
	глПараметрыСканераШтрихкодовAndroid.СобытиеОбработано = Истина;
	
	Данные = Новый Массив;
	Данные.Добавить(ПреобразоватьДанныеСоСканераВСтруктуру(Параметр));
	
	Возврат Данные;
	
КонецФункции

Функция ПреобразоватьДанныеСоСканераВСтруктуру(Параметр) Экспорт
	
	глПараметрыСканераШтрихкодовAndroid.СобытиеОбработано = Истина;
	
	Если Параметр[1] = Неопределено Тогда
		Данные = Новый Структура("Штрихкод, Количество", Параметр[0], 1);    // Достаем штрихкод из основных данных
	Иначе
		Данные = Новый Структура("Штрихкод, Количество", Параметр[1][1], 1); // Достаем штрихкод из дополнительных данных
	КонецЕсли;
	
	Возврат Данные;
	
КонецФункции

// Устанавливает параметры сканера в глобальную переменную "глПараметрыСканераШтрихкодовAndroid" модуля приложения для
// кеширования.
//
Процедура УстановитьПараметрыСканера() Экспорт
	
	глПараметрыСканераШтрихкодовAndroid = МенеджерСканераШтрихкодовAndroidВызовСервера.ПолучитьПараметрыСканера();
	
КонецПроцедуры 

#КонецОбласти 

#Область СлужебныеПроцедурыИФункции

Процедура ИнициализироватьУстройство(КонтекстПодключения)
	
	ИдУстройства = "";
	Оповещение = Новый ОписаниеОповещения("ИнициализироватьУстройствоЗавершение", ЭтотОбъект, КонтекстПодключения);
	КонтекстПодключения.ОбъектДрайвера.НачатьВызовПодключить(Оповещение, ИдУстройства);
	
КонецПроцедуры 

Процедура ИнициализироватьУстройствоЗавершение(РезультатВыполнения, ПараметрыВызова, КонтекстПодключения) Экспорт
	
	Клиенты = Новый Массив;
	Клиенты.Добавить(КонтекстПодключения.ИдентификаторКлиента);
	
	Сканер = Новый Структура;
	Сканер.Вставить("Клиенты", Клиенты);
	Сканер.Вставить("КоличествоПодключенных", 1);
	Сканер.Вставить("ОбъектДрайвера", КонтекстПодключения.ОбъектДрайвера);
	
	глПараметрыСканераШтрихкодовAndroid.Сканер = Сканер;
	
КонецПроцедуры 

Функция МестоположениеДрайвера()
	
	Возврат "ОбщийМакет.ДрайверСканераШтрихкодовAndroid";
	
КонецФункции 

Процедура НачатьПолучениеОбъектаДрайвера(ОповещениеОЗавершении)
	
	ОповещениеОПолученииДрайвера = Новый ОписаниеОповещения(
		"ПолучениеОбъектаДрайвераЗавершение",
		ЭтотОбъект,
		ОповещениеОЗавершении);
	НачатьПодключениеДрайвера(ОповещениеОПолученииДрайвера);
	
КонецПроцедуры 

Процедура ОтключениеЗавершение(РезультатВыполнения, ПараметрВызова, ДопПараметры) Экспорт
	
	глПараметрыСканераШтрихкодовAndroid.Сканер = Неопределено;
	
КонецПроцедуры 

Процедура ПодключениеЗавершение(ОбъектДрайвера, ИдентификаторКлиента) Экспорт
	
	Если ОбъектДрайвера = Неопределено Тогда
		Возврат;
	КонецЕсли; 
	
	Параметры = Новый Структура;
	Параметры.Вставить("ActionName", глПараметрыСканераШтрихкодовAndroid.ActionName);
	Параметры.Вставить("ExtraData", глПараметрыСканераШтрихкодовAndroid.ExtraData);
	
	МассивПараметров = Новый Массив;
	МассивПараметров.Добавить(Новый Структура(
		"ИмяПараметра, Значение",
		"ActionName", глПараметрыСканераШтрихкодовAndroid.ActionName));
	МассивПараметров.Добавить(Новый Структура(
		"ИмяПараметра, Значение",
		"ExtraData", глПараметрыСканераШтрихкодовAndroid.ExtraData));
	
	КонтекстПодключения = Новый Структура;
	КонтекстПодключения.Вставить("Параметры", МассивПараметров);
	КонтекстПодключения.Вставить("ТекущийПараметр", 0);
	КонтекстПодключения.Вставить("ИдентификаторКлиента", ИдентификаторКлиента);
	КонтекстПодключения.Вставить("ОбъектДрайвера", ОбъектДрайвера);
	
	УстановкаПараметраСканераЗавершение(Истина, Неопределено, КонтекстПодключения);
	
КонецПроцедуры 

Процедура ПолучениеОбъектаДрайвераЗавершение(Подключено, ОповещениеОЗавершении) Экспорт
	
	Если Не Подключено Тогда
		Сообщение = Новый СообщениеПользователю;
		Сообщение.Текст = НСтр("ru='Ошибка при подключении драйвера сканера штрихкодов Android. Сканирование не доступно.'");
		Сообщение.Сообщить();
		
		ВыполнитьОбработкуОповещения(ОповещениеОЗавершении, Неопределено);
		Возврат;
	КонецЕсли; 
	
	Попытка
		ProgID = "AddIn.com_ptolkachev_AndroidScannerExtension.com_ptolkachev_AndroidScannerExtension";
		ОбъектДрайвера = Новый(ProgID);
		ВыполнитьОбработкуОповещения(ОповещениеОЗавершении, ОбъектДрайвера);
	Исключение
		ТекстОшибки = СтрШаблон(
			НСтр("ru='Ошибка при подключении драйвера сканера штрихкодов Android: %1. Сканирование не доступно.'"),
			КраткоеПредставлениеОшибки(ИнформацияОбОшибке()));
		
		Сообщение = Новый СообщениеПользователю;
		Сообщение.Текст = ТекстОшибки;
		Сообщение.Сообщить();
		
		ВыполнитьОбработкуОповещения(ОповещениеОЗавершении, Неопределено);
	КонецПопытки; 
	
КонецПроцедуры 

Процедура УстановкаПараметраСканераЗавершение(РезультатВыполнения, ПараметрВызова, КонтекстПодключения) Экспорт
	
	ВсеПараметрыУстановлены = (КонтекстПодключения.ТекущийПараметр = КонтекстПодключения.Параметры.Количество());
	Если ВсеПараметрыУстановлены Тогда
		ИнициализироватьУстройство(КонтекстПодключения);
		Возврат;
	КонецЕсли; 
	
	Параметр = КонтекстПодключения.Параметры[КонтекстПодключения.ТекущийПараметр];
	КонтекстПодключения.ТекущийПараметр = КонтекстПодключения.ТекущийПараметр + 1;
	
	Оповещение = Новый ОписаниеОповещения("УстановкаПараметраСканераЗавершение", ЭтотОбъект, КонтекстПодключения);
	КонтекстПодключения.ОбъектДрайвера.НачатьВызовУстановитьПараметр(
		Оповещение, Параметр.ИмяПараметра, Параметр.Значение);
	
КонецПроцедуры 

#КонецОбласти 

#ifndef FORMAT_H
#define FORMAT_H

#include <string>
#include <stdexcept>
#include <vector>
#include <math.h>

#define DEFAULT_PRECISION (-1)  //default value
#define WP_READ (-2)    // useful if we want to read a width or a precision value from the arguments list

namespace formatImpl
{
	//Спецификаторы формата
	enum FormatSpec
	{	
		d,	//Десятичное целое число со знаком	
		u,	//Десятичное целое число без знака
		e,	//Экспоненциальное представление десятичного числа (e на нижнем регистре)
		E,	//Экспоненциальное представление десятичного числа (E на верхнем регистре)
		f,	//Десятичное число с плавающей точкой
		F,	//Десятичное число с плавающей точкой
		g,	//Использует более короткий из форматов %e или %f
		G,	//Использует более короткий из форматов %E или %F
		o,	//Восьмеричное число без знака	
		a,	//Шестнадцатеричное число в форме 0xh.hhhhp+d
		A,	//Шестнадцатеричное число в форме 0Xh.hhhhP+d
		x,	//Шестнадцатеричное без знака (строчные буквы)
		X,	//Шестнадцатеричное без знака (прописные буквы)
		c,	//Символ
		s,	//Символьная строка

		p, //Выводит указатель
		n, //Соответствующий аргумент должен быть указателем на целое число. (Этот спецификатор указывает, что в целочисленной переменной, на которую указывает ассоциированный с данным спецификатором указатель, будет храниться число символов, выведенных к моменту обработки спецификации %n

		//Спецификаторы длинны
		l,	
		ll,
		hh,
		h,	
		j,
		z,
		t,
		L,
		def,
		automatic,
	};

	struct Format
	{
		std::string str;
		union
		{
			int flags = 0;
			struct {
				bool zero : 1;    //'0' Заполняет пустое место поля нулями (по умолчанию пробелами)
				bool plus : 1;    //'+' Добавляет знак "+" в начале числа, если оно положительное
				bool minus : 1;   //'-'	Выравнивание по левому краю поля (по умолчанию по правому)
				bool sharp : 1;   //'#' Для восьмиричных добавляет приставку 0; для шестнадцатиричных добавляет приставку 0х; для десятичных записывает десятичную точку, даже если её нет
				bool space : 1;   //' ' Заполняет пустое место поля пробелами
			};
		};
		int width = 0;	//Минимальная ширина поля вывода
		int precision = DEFAULT_PRECISION;	//Точность числа (количество цифр после запятой)
		FormatSpec length;	//Спецификатор длинны
		FormatSpec type;	//Спецификатор формата
	};

	void parseFormatString(std::vector<Format> &fmt, const char *format);



	template<typename T>
	//Генерирует десятичное целое число со знаком
	std::string sprintDec(Format const *fmt, T arg)
	{
		std::string result;
		const char* signStr;
		int m = (int)arg;

		//знак числа
		if (m < 0)
		{
			signStr = "-";
			m = -m;
		}
		else
			signStr = (fmt->plus ? "+" : "");

		//получаем цифры числа
		while (m>0)
		{
			char symbol = (m % 10);
			if (symbol < 0)
				symbol = -symbol;
			symbol += '0';
			result=symbol+result;
			m /= 10;
		}
		if (result.size() == 0)
			result = "0";

		result = signStr + result;

		return result;
	}

	template<typename T>
	//Генерирует десятичное целое число без знака
	std::string sprintUnsDec(Format const *fmt, T arg)
	{
		std::string result;
		const char* signStr;
		unsigned m = (unsigned)arg;

		signStr = (fmt->plus ? "+" : "");

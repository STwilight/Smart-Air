/*
 * Functions.h
 *
 *  Created on: 24 окт. 2016 г.
 *      Author: Symrak
 */

#ifndef APP_FUNCTIONS_H_
#define APP_FUNCTIONS_H_

String convertMAC(String macAddress, bool fullMAC = true)
{
	/* Метод для конвертации MAC адреса с разделением двоеточием.
	 *
	 * При значении входного параметра "fullMAC" равному "true",
	 * 		производится конвертация полного MAC-адреса,
	 * 		в противном случае конвертируются лишь три последних байта.
	 *
	 *  */

	String result = macAddress;
	result.toUpperCase();

	char macChar[macAddress.length() + 1];
	result.toCharArray(macChar, macAddress.length() + 1);

	result = "";
	byte size = sizeof(macChar) - 1;

	int startNUM = 0;
	if(!fullMAC)
		startNUM = 6;

	for(int i=startNUM; i<size; i++) {
		result += macChar[i];
		if((i%2 != 0) & (i != size-1))
			result += ":";
	}

	return result;
}
String convertHEX(String hexSN)
{
	/* Метод конвертации HEX строки вида "1a2b3c" в DEC значение */

	const char symbols[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
							  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

	String hexNumber = hexSN;
	byte hexLength = hexNumber.length();
	uint32_t decResult = 0;

	hexNumber.toUpperCase();
	for(int i=0; i<hexLength; i++)
		for(int j=0; j<sizeof(symbols); j++) {
			if(hexNumber.charAt(i) == symbols[j])
				decResult += j << 4*(hexLength-i-1);
	}

	return String(decResult);
}
String convertSN(String macAddress)
{
	/* Метод конвертации 3 последних байт MAC-адреса в десятичный серийный номер */

	String tmp;

	char macChar[macAddress.length() + 1];
	macAddress.toCharArray(macChar, macAddress.length() + 1);

	byte size = sizeof(macChar) - 1;
	for(int i=6; i<size; i++)
		tmp += macChar[i];

	/* Необходимо использовать конвертацию из String в uint32_t для преобразования HEX > DEC
	 * 		посредством использования недоступной сейчас в Sming функции sscanf().
	 *
	 * Временное решение – самописный конвертер HEX-кода в DEC String значение.
	 * Функция для вызова: (String) convertHEX(String hexSN).
	 *
	 * Правильное решение:
	 * 		sscanf(tmp.c_str(), "%x", &result);
	 * 		return (long) result;
	 *
	 */

	return convertHEX(tmp);
}

#endif /* APP_FUNCTIONS_H_ */
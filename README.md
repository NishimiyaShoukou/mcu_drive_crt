# <center> STM32 HAL ����CRT��ʾ������Bad Apple

## ���
&emsp;����Ŀʹ��STM32F103C8T6��Ƭ�����NTSC�źţ�ͨ��RCA�ӿ�����CRT��ʾ��.
����ASII���ֿ⣬����ʵ�ּ򵥵��ַ�����ʾ��ͼƬ��ʾ���ܡ�������STM32F103C8T6�����Լ�û��DACԭ��MCUֻ������ڰ�ͼ���źš�

&emsp;����USB���⴮�ڡ���λ��ֱ��ͨ��USB����STM32���Ŵ������ݣ��Ӷ�ͨ��USBͨ�Ŵ���ͼ������ʵʱ����Bad Apple��Ƶ��

![Ч��ͼ](./images/01.jpg)

***  

## �ο�
__�ο������������Ŀ:__
1. ��ʾ����: [ArduinoSTM32_TNTSC][1]  

2. ��λ��&emsp;: [oled_display_gui][2]

***  

## ����׼��
> 1. Ӳ��׼����
> >  - STM32f103C8T6, 560��240���ĵ���, CRT��ʾ��, RCA��
> 2. ���׼����
> >  - KEIL5 �� STM32CuBeMX, OLED_ToolBox


***  

## ����ͼ  

![����ͼ](./images/02.jpg)  

***  

## ע��  

&emsp;������λ������ͼ����Ҫ�������洢�������ʾ�ֱ��ʲ���̫�� �����ڴ�ᱬ! ĿǰΪSC_224x108�ֱ��ʡ�

***  

## ͼ������  

&emsp;�����ʽ������100000���ֱ��ʲ���160*80������ɨ�裬�ֽں������С�LSB
&emsp;ntsc.c�ļ�����������ýӿ�

[1]: https://github.com/Tamakichi/ArduinoSTM32_TNTSC#arduino-stm32-ntsc%E3%83%93%E3%83%87%E3%82%AA%E5%87%BA%E5%8A%9B%E3%83%A9%E3%83%96%E3%83%A9%E3%83%AA
[2]: https://github.com/AnChangNice/oled_display_gui

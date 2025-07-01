Descriptivo del Plugin de Filtros Pasa Bajo y Pasa Alto en Python
Objetivo:
Desarrollar un plugin de audio en Python que implemente dos filtros básicos: un filtro pasa bajo (LPF) y un filtro pasa alto (HPF), con la capacidad de controlar sus frecuencias de corte en tiempo real a través de una interfaz gráfica sencilla.
Descripción Técnica:
1.	Filtros de Audio:
o	Se utilizarán filtros digitales de primer orden para crear el pasa bajo (LPF) y el pasa alto (HPF).
o	El filtro pasa bajo permite que las frecuencias por debajo de una frecuencia de corte pasen, mientras que el filtro pasa alto permite que las frecuencias por encima de la frecuencia de corte pasen.
o	Fórmulas para los filtros:
	Pasa Bajo (LPF):
y[n]=(1−α)⋅x[n]+α⋅y[n−1]y[n]=(1−α)⋅x[n]+α⋅y[n−1]
Donde α=11+fcsample rateα=1+sample ratefc1, siendo fcfc la frecuencia de corte.
	Pasa Alto (HPF):
y[n]=α⋅(y[n−1]+x[n]−x[n−1])y[n]=α⋅(y[n−1]+x[n]−x[n−1])
Donde α=fcfc+sample rateα=fc+sample ratefc.
2.	Controles:
o	Se proporcionará una interfaz gráfica en Tkinter para permitir al usuario controlar:
	La frecuencia de corte de los filtros.
	El tipo de filtro (pasa bajo o pasa alto).
o	Los controles estarán en tiempo real, permitiendo al usuario experimentar con diferentes frecuencias y configuraciones de filtro.
3.	Procesamiento de Audio:
o	Se utilizará la biblioteca PyAudio para capturar el audio de entrada y reproducir el audio procesado en tiempo real.
o	El plugin aplicará el filtro adecuado a la señal de audio, dependiendo de la opción seleccionada por el usuario.
________________________________________
Plan de Desarrollo:
1.	Configuración del entorno:
o	Instalar las bibliotecas necesarias para procesamiento de audio y creación de la interfaz gráfica.
o	PyAudio para capturar y procesar el audio.
o	Tkinter o PyQt para la interfaz gráfica sencilla.
bash
CopyEdit
pip install pyaudio numpy scipy
2.	Diseño del Plugin:
o	Implementar dos filtros: LPF y HPF utilizando las fórmulas mencionadas.
o	Crear una interfaz gráfica en Tkinter para controlar los parámetros en tiempo real.
3.	Desarrollo de la Interfaz Gráfica:
o	Crear un control deslizante para ajustar la frecuencia de corte.
o	Crear un control para seleccionar el tipo de filtro (pasa bajo o pasa alto).
o	Actualizar los filtros en tiempo real mientras el usuario ajusta los controles.
4.	Desarrollo del Procesador de Audio:
o	Usar PyAudio para capturar audio desde el micrófono o la entrada de línea.
o	Procesar el audio en tiempo real aplicando los filtros seleccionados por el usuario.
o	Reproducir el audio procesado.

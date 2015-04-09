Contador de Ciclistas
=====================

Projeto experimental de contagem de ciclistas utilizando webcam de baixo custo.

Requisitos
-----------

- OpenCv 2.4
- V4l2Loopback
- WebCam

Como Usar
---------

- Compilar a aplicação digitando na linha de comando:
make

- Iniciar a aplicação utilizando a linha de comando abaixo:

`bin/CycloTracker usage:`

	--help   (-h): print this message. 
	--override (-O): override point picked parameters.
	--reg_source (-s) <file_name>: Specify regular file where data comes from.
	--dev_source (-D) <number>: Specify device number where data comes from.
	--record (-r) <file_name>: Record video to filename.
	--stream (-S) <device>: Streams video via device.
	--sensor (-t) <device>: Specify device file which is a sensor.
	--address (-a) <address>: address must be present and must be in in this format: street-number. --address faria_lima-1200.

Exemplo:

`bin/CycloTracker -D 0 --address Faria_Lima-2152 --record out.avi -S /dev/video1`

- Necessário selecionar (clicando na tela com o endereço fornecido):

1) O ponto em que o contador amarelo de ciclistas da direita para a esquerda deve se localizar

2) O ponto em que o contador amarelo de ciclistas da esquerda para a direita deve se localizar

3) O ponto superior esquerdo da área em perspectiva 

4) O ponto superior direito da área em perspectiva

5) O ponto inferior esquerdo da área em perspectiva

6) O ponto inferior direito da área em perspectiva

7) O ponto superior esquerdo da área de corte (vermelha)

8) O ponto inferior direito da área de corte (vermelha)


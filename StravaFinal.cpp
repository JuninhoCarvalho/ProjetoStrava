#include<iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
using namespace std;

//struct com as variaveis
struct dados
{
	int horas; //horas
	double lat; //latitude
	double lon; //longitude
	int minutos; // minutos
	double ele; //elevacao
	int segundos; //segundos
};

//Função calculo a distancia total
double distancia(double lat1, double long1, double lat2, double long2)
{
	float r = 6371;
	float calcRad = 3.1415926536 / 180;
	double dist; //distancia
	long1 *= calcRad, long2 *= calcRad, lat1 *= calcRad, lat2 *= calcRad;
	dist = r * acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(long2 - long1));
	return dist;
}

//Calculo da velocidade media
double velocidade (double distotal, int tempoI, int tempoF)
{
	double vmedia; // velociidade media
	vmedia = 3600 * distotal  / (tempoF - tempoI);
	return vmedia;
}

//Funcao com parametro e retorno que calcula o tempo total
int segundos(int hora, int minuto, int segundo)
{
	int tempot;
	tempot = (hora * 3600) + (minuto * 60) + segundo;
	return tempot;
}
//calculo do tempo
void tempo(int segundos, ofstream &arqS)
{
	int hora, minuto, segundo;
	hora = segundos / 3600;
	minuto = (segundos % 3600)/60;
	segundo = segundos % 60;
	arqS << "Tempo total decorrido: " << hora << ":" << minuto << ":" << segundo << endl;
}
int main()
{
	ifstream arqL;//var arqL para leitura do arquivo
	ofstream arqS;//var arqS para saida em outro arquivo
	
	arqL.open("leitura.txt",ifstream::in);//abrindo o arquivo de leitura dos dados
	arqS.open("saida.txt",ofstream::out);//abrindo o arquivo de saida dos dados   	

	//variaveis
	dados p1, p2;
	double dTotal = 0; //distancia total
	double ganho = 0, perda = 0; //ganho e a perde
	double alto, baixo, vel;
	double vmax = 0, vmedia; // velocidade maxima e media
	int tempo1, tempo2, t, tinicial, tfinal; // variaveis de tempo

	// Entrando com os dados
	arqL >> p1.lat >> p1.lon >> p1.ele >> p1.horas >> p1.minutos >> p1.segundos;

	tinicial = segundos(p1.horas, p1.minutos, p1.segundos);

	cout << fixed << setprecision(6);
	alto = p1.ele;
	baixo = p1.ele;
	do
	{	//Leitura das informacoes contidas no arquivo
		arqL >> p2.lat >> p2.lon;

		//Condições para que o loop continue
		if(!(p2.lat != 0.0 && p2.lon != 0.0))
			break;
		arqL >> p2.ele >> p2.horas >> p2.minutos >> p2.segundos;

		dTotal += distancia(p1.lat, p1.lon, p2.lat, p2.lon);
		tempo1 = segundos(p1.horas, p1.minutos, p1.segundos);
		tempo2 = segundos(p2.horas, p2.minutos, p2.segundos);
		t = tempo2 - tempo1;
		vel = 3600 * distancia(p1.lat, p1.lon, p2.lat, p2.lon) / t;
		if (vel > vmax)
			vmax = vel;
		if(p2.ele > p1.ele)
			ganho += p2.ele - p1.ele;
		else
			perda += p2.ele - p1.ele;
		if(p2.ele > alto)
			alto = p2.ele;
		if(p2.ele < baixo)
			baixo = p2.ele;
		p1 = p2;
		tfinal = segundos(p1.horas, p1.minutos, p1.segundos);
	}
	while(p1.lat != 0.0 && p1.lon != 0.0 && p2.lat != 0.0 && p2.lon != 0.0);

	vmedia = velocidade(dTotal, tinicial, tfinal);
	
	//Saida dos resultados gerados pelo programa no arquivo "Saida.txt"
	arqS << fixed << setprecision(2);
	arqS << "Distancia total: " << dTotal << " km" << endl;//saida da Distancia total
	tempo(tfinal - tinicial, arqS);//chamada da funcao sem retorno com parametro e por referencia para saida do tempo final
	arqS << "Velocidade maxima: " << vmax << " km/h" << endl;//velocidade maxima
	arqS << "Velocidade media: " << vmedia << " km/h" << endl;//velocidade media
	arqS << "Ganho de elevacao: " << ganho << " m" << endl;//Ganho da elevacao
	arqS << "Perda de elevacao: " << perda << " m" << endl;//perda da elevacao
	arqS << "Maior altitude: " << alto << " m" << endl;//Maior altitude registrada
	arqS << "Menor altitude: " << baixo << " m" << endl;//Menor altitude registrada
	
	//fechando os arquivos que foram abertos para leitura e saida dos dados
	arqL.close();
	arqS.close();

	return 0;
}

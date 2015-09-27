/*Directivas del procesador*/
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <cstdlib>
/*Definimos la funcion para rendondear un numero decimal al entero mas cercano*/
int redondeo(double x){
	if ((x-floor(x))<=0.5) {
		x = floor(x);
	}
	else {
		x = ceil(x);
	}
	return (int)x ;
}
/*SALIDA:Estructura conteniendo las Medidas de Tendencia Central
* Promedio, mediana y desviacion estandar(sigma)
*/
struct mtc {
	float promedio,mediana,sigma,moda;
};
/*Estructura para guardar las modas*/
struct array {
	float x; int y;
};
/*ENTRADA:Estructura conteniendo los datos de entrada
*/
struct datos {
int N/*Numero de datos*/;
float * data/*Arreglo de datos que se dan*/,
/*--------------NO SON DE SALIDA PERO UTILES-------------*/
*_data/*Arreglo de datos para el calculo de la moda*/;
array* moda/*Arreglo de modas*/;
/*-------------------------------------------------------*/
};
/*SALIDA:Estructura conteniendo las variables para datos agrupados*/
struct Data_group{
	float /*Numero de intervalos*/n_inter,
	/*longitud de cada intervalo*/l_inter;
	float /*arreglo con los intervalos*/*inter,
  /*arreglo con las frecuencias en cada intervalos*/*f,
	/*arreglo con las frecuencias relativas en cada intervalos*/*f_r,
	/*arreglo con las marcas cada intervalos*/*marca,
	/*arreglo con las frecuencias acumuladas en cada intervalos*/*f_acum,
	/*arreglo con las frecuencias acumuladas relativas en cada intervalos*/*f_acum_r;
};
/*Estructura conteniendo las variables a usar en el codigo*/
struct variables{
	mtc MTC;
	datos in;
	Data_group group;
	int dist;
	float sum,menor ,error ;
	bool par;
};
/*funcion que prdena un conjunto de datos*/
void ordena(variables* _this){
	int i,j;
	for ( j = 1; j < _this->in.N; j++) {
		for ( i = j; i >= 1; i--) {
			if (_this->in.data[i]<= _this->in.data[i-1]) {
				_this->menor = _this->in.data[i];
				_this->in.data[i]= _this->in.data[i-1];
				_this->in.data[i-1] = _this->menor;
			}else {
					break;
			}
		};
	};
}
/*funcion que calcula la mediana*/
void median(variables *_this){

	_this->dist = _this->in.N%2;
	if (_this->dist == 0) {
		_this->par = true;
	};
	if (_this->par) {
		_this->MTC.mediana = (_this->in.data[_this->in.N/2-1]+_this->in.data[_this->in.N/2])/2;
	}
	else {
		_this->MTC.mediana = _this->in.data[(_this->in.N+1)/2-1];
	};

}
/*funcion que pide los datos*/
void input(variables *_this){
	int i;
	printf ("Ingresa número de datos:");
	scanf("%i",&_this->in.N);
	/*definimos el tamaño del array para los datos*/
	_this->in.data = new float[_this->in.N];
	printf("Ingresa los datos \n");
	printf("Presiona enter al ingresa cada uno.\n");
	for ( i = 0; i < _this->in.N; i++) {
		printf("N_%i=",i);
		scanf("%f",&_this->in.data[i] );
		_this->sum +=_this->in.data[i];
	}

}
/*funcion que pide los datos*/
void finput(variables *_this){
	int i,numData,j; FILE *myfile; float *Data;
	/*definimos el tamaño del array para los datos*/
	printf ("Ingresa número de datos tomados de la problacion:");
	scanf("%i",&_this->in.N);
	Data = new float[10000];
		_this->in.data = new float[_this->in.N];
	i=0; myfile = fopen("data_2.dat" ,"r");
	if ( ferror(myfile) !=0)
	{
		printf("Error: no se puede abrir el fichero\n");
	} else{
		while (feof(myfile)==0)
		 /* mientras no se llegue al finaf def fichero */
		{
			fscanf(myfile,"%f",&Data[i] );
			_this->sum +=	Data[i];
			i++;
		}
		numData= i+1;
		for ( j = 0; j < _this->in.N; j++) {
			i = redondeo(rand()%numData )		;
			_this->in.data[j] = Data[i]			;
		}
		fclose(myfile);
	}


}
	/*funcion que calcula las variables para datos agrupados*/
	void Grouped(variables *_this){
	int i,j,count;
	/*Calculamos el numero de intervalos*/
	_this->group.n_inter =(float) redondeo(sqrt((double) _this->in.N));
	/*Definimos el tamaño de los arreglos a usar en los datos agrupados
	* mediante el uso de memoria dinamica
	*/
	_this->group.inter 	 = new float[(int) _this->group.n_inter+1];
  _this->group.f		 	 = new float[(int) _this->group.n_inter];
	_this->group.f_r	 	 = new float[(int) _this->group.n_inter];
	_this->group.marca 	 = new float[(int) _this->group.n_inter];
	_this->group.f_acum	 = new float[(int) _this->group.n_inter];
	_this->group.f_acum_r= new float[(int) _this->group.n_inter];
	/*Calculamos la longitud de cada intervalo*/
	_this->group.l_inter = (_this->in.data[_this->in.N-1]-_this->in.data[0])/_this->group.n_inter;
	_this->group.inter[0]=_this->in.data[0];
	for (i = 1;  i<= _this->group.n_inter; i++) {
		_this->group.inter[i] = _this->group.inter[i-1]+_this->group.l_inter ;
		_this->group.marca[i-1]= _this->group.inter[i-1]+_this->group.l_inter/2;
	}
	count=0;
	for (i = 1; i <= _this->group.n_inter; i++) {
		for ( j = count; j < _this->in.N; j++) {
				/*Determinamos el la freceuncia de cada intervalo*/
				if ( _this->group.inter[i-1]<= _this->in.data[j] && _this->in.data[j] <= _this->group.inter[i]) {
					_this->group.f[i-1]++;
					/*Calculamos el error de cada dato*/
					_this->error += pow((_this->in.data[j]-_this->MTC.promedio),2);
				}else{
					count = j;
					break;
				}
		}
		/*Calculamos la frecuencia relativa*/
		_this->group.f_r[i-1]=_this->group.f[i-1]/_this->in.N;
		/*Calculamos la frecuencia acumulada*/
		if ((i-1)==0) {_this->group.f_acum[i-1]=_this->group.f[0];}else{
		_this->group.f_acum[i-1]=_this->group.f[i-1]+_this->group.f_acum[i-2];}
		/*Calculamos la frecuencia relativa acumulada*/
		_this->group.f_acum_r[i-1]=_this->group.f_acum[i-1]/_this->in.N;
	}
}
/*funcion que pide los datos*/
void output(variables *_this){
	int i;
	/*Los resultados son:*/
	printf("{");
	for ( i = 0; i < _this->in.N; i++) {
	printf("%.2f \t",_this->in.data[i]);
	}
	printf("}\n");
	printf("Num de datos %i \nPromedio %.2f \n Mediana %.2f \n Moda %.2f\n Desviación Estánda %.2f\n",_this->in.N,_this->MTC.promedio, _this->MTC.mediana,_this->MTC.moda, _this->MTC.sigma );
	/*Si los datos son agrupados*/
	printf("Si los datos son agrupados:\n" );
	printf("|intervalos | marca | frecuencia | frecuencia Acumulada | frecuencia relativa | frecuencia relativa acumulada|\n" );
	for (i = 1; i <= _this->group.n_inter; i++) {
		printf("|%.2f-%.2f| %.2f| %.2f	| %.2f|\
	%.2f| %.2f|\n",_this->group.inter[i-1],_this->group.inter[i],_this->group.marca[i-1],\
	_this->group.f[i-1],_this->group.f_acum[i-1],_this->group.f_r[i-1],_this->group.f_acum_r[i-1] );
	}

}
void modoinput(variables *_this) {
	int des;
	printf("Tus datos son via teclado o archivo??\n" );
	printf("1. Para pantalla \n 2. Para archivo \n" );
	scanf("%i",&des );
	if(des >=2){
		/*Leemos los datos desde el archivo*/
		finput(_this);
    }else{
	/*Pedimos los datos de entrada y calculamos la suma de los datos*/
		input(_this);
	}
}
void moda(variables *_this) {
	int i,count,nmoda,j=0;
	float *_data_;
	_data_=new float[_this->in.N];
	/*Hacemos una copia de los datos*/
	for (i = 0; i < _this->in.N; i++) {
		_data_[i]=_this->in.data[i];
	}
	/*Contamos los datos que faltan por revisar*/

	count = _this->in.N;
	_this->in._data = new float[count];
	/*Guardamos los datos de la moda en un arreglo*/

	_this->in.moda = new array[count];
	while ( count>1) {
		/*revisamos de abajo para arriba*/

		_this->in.moda[j].x = _data_[0];
		_this->in.moda[j].y = 1;

		if (j==0) {
				_this->MTC.moda = _this->in.moda[j].x;
		}

		for (i = 1; i < count; i++) {

			if(_data_[i]==_this->MTC.moda){

					_this->in.moda[j].y++;
			} else{

				_this->in._data[j]=_data_[i];
			}
		}

		if(j==0){
			nmoda =_this->in.moda[j].y;
		}

		if(nmoda < _this->in.moda[j].y){
			_this->MTC.moda =_this->in.moda[j].x;
		}

		count -= _this->in.moda[j].y ;
		j++;
			/*Hacemos de nuevo la copia de los datos*/
		for (i = 0; i < count; i++) {
			_data_[i]=_this->in._data[i];
		}
	}

	if (count ==1) {
		_this->in.moda[j].x =_data_[0];
		_this->in.moda[j].y = 1;
	}

}
/*Aqui inicia el programa*/
int main ()
{

	/*Definimos las variables a usar y las escojemos a sus valores iniciales*/
	variables _this; _this.par = true;
	_this.sum =0;_this.error = 0;
	/*Pedimos los datos*/

	modoinput(&_this);
	/*Ordenamiento de los datos de menos a mayor*/

	ordena(&_this);
 	/*Calculo de la mediana Determinamos si el numero de datos es par*/

 	median(&_this);
	/*Calculamos las variables para datos agrupados y el error de los datos*/

	Grouped(&_this);
	/*Calculamos la moda*/

	moda(&_this);
	/*Calculo del promedio*/
	_this.MTC.promedio = _this.sum / _this.in.N;
	/*Determinamos la desviacion estandar*/
	_this.MTC.sigma = sqrt(_this.error/_this.in.N);
	/*imprimimos los resultado*/
	output(&_this);
	return 0;
}

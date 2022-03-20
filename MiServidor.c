#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// escucharemos en el port 9050
	serv_adr.sin_port = htons(9051);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	//La cola de peticiones pendientes no podr? ser superior a 4
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	
	// bucle infinito
	for(;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexi?n\n");
		//sock_conn es el socket que usaremos para este cliente
		
		//BUcle de atención al cliente
		int terminar =0;
		int mayus =0;
		while(terminar == 0)
		{
		
			// Ahora recibimos su nombre, que dejamos en peticion
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
			
			// Tenemos que a?adirle la marca de fin de string 
			// para que no escriba lo que hay despues en el buffer
			peticion[ret]='\0';
			
			//Escribimos el nombre en la consola
			
			printf ("Se ha conectado: %s\n",peticion);
			
			
			char *p = strtok( peticion, "/");
			int codigo =  atoi (p);
			char nombre[20];
			
			if (codigo !=0)
			{
				p = strtok( NULL, "/");
		
				strcpy (nombre, p);
				printf ("Codigo: %d, Nombre: %s\n", codigo, nombre);
			}
			if (codigo ==0)
				terminar =1;
			
			else if (codigo ==1)
				//piden la longitd del nombre
				sprintf (respuesta,"%d",strlen (nombre));

			else if (codigo ==2)
				// quieren saber si el nombre es bonito
				if((nombre[0]=='M') || (nombre[0]=='S'))
					strcpy (respuesta,"SI,");
				else
					strcpy (respuesta,"NO,");
				
			else if (codigo ==3){
				//decir si es alto
				p = strtok( NULL, "/");
				float altura  =  atof (p);
				if (altura > 1.70)
					sprintf (respuesta, "%s: eres alto",nombre);
				else
					sprintf(respuesta,"%s:eres bajo",nombre);
			}	
			else if (codigo ==4){
				//decir si el nombre es palindromo
			/*	char b= ' ',nombre1[20], nombre2[20];
				strcpy (nombre, nombre1);
				strcpy (nombre, nombre2);
				int n = strlen(nombre1);
				int e = 0, x, a = 0, p = 0;
			 
			 for(int i=0;i<=n;i++)
			 {
				 if (nombre1[i] == b)
					 i++;
				nombre2[e]=nombre1[i];
				e++;
			 }
			 for (x = n-1; x>=0;x--)
			 {
				 printf("\n %c \t", nombre1[x]);
				 x=0;
				 n = strlen(nombre2);
				 for(x=n-1;x>=0;x--){
					 if(nombre2[x] == nombre2[a])
						 p++;
					a++;
					
				 }
				 if(p == n)
					sprintf(respuesta,"%s tu nombre SI es un palindromo",nombre);
				 else
					sprintf(respuesta,"%s tu nombre NO es un palindromo",nombre);
				   
			 }
				*/
				
				/*if(strlen(nombre)>1)
				{
					if(nombre[0] == nombre[strlen(nombre)-1])
						
				}
				*/
				int posin, posfin, i=0;
				char nombre1[20];
				strcpy(nombre,nombre1);
				if(strlen(nombre1)>1)
				{
					if(nombre1[0] == nombre1[strlen(nombre1)-1]){
						/*while(i<strlen(nombre)&&nombre !="\n")
						{
							nombre1[i] = nombre;
							i++;					
						}*/
						posin = 0;
						posfin = i-1;
						
						while(posin<posfin && nombre1[posin] == nombre1[posfin])
						{
							posin++;
							posfin--;
						}
						if(posin>=posfin)
							sprintf(respuesta, "%s tu nombre SI es palindromo",nombre);	
					}
				}
				else
					sprintf(respuesta, "%s tu nombre NO es palindromo",nombre);
			}
			
			else if (codigo ==5){
				//quieren su nombre en mayúsculas
				// Convertir cada char a mayúscula
				// usando toupper
				for (int indice = 0;nombre[indice] != '\0'; ++indice){
					nombre[indice] = toupper(nombre[indice]);
				}
				mayus = 1;
				if (mayus==1)
					sprintf(respuesta,"Tu nombre en mayusculas es: %s", nombre);
			}		
			
			if (codigo!=0)
			{
				printf ("%s\n", respuesta);
				// Y lo enviamos
				write (sock_conn,respuesta, strlen(respuesta));
			}
				
				
		}
		// Se acabo el servicio para este cliente
		close(sock_conn); 
	}
}

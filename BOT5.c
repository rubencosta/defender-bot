#pragma config(Sensor, S1,     sensorCor,      sensorCOLORFULL)
#pragma config(Sensor, S2,     sensorToque,    sensorTouch)
#pragma config(Sensor, S4,     sensorStart,    sensorTouch)
#pragma config(Motor,  motorA,          motorToque,    tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          motorBase,     tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          motorEmpurra,  tmotorNXT, PIDControl, encoder)

//definição do atacante
typedef struct
{
  int tipo;  //0 vazio; 1 Tanque; 2 Artilharia; 3 Infantaria
  int forca;
  float vida;
  int ataques;
  int vidaInicial;
  int prioridade;
} inimigo;

//constantes
//ambiente
const int turnos = 6;
const int nslots = 6;
//robot
const int vidaInicial = 750;
const float energiaInicial = 500;
//ataques
const int danoGrua = 200;
const int danoToque = 100;
const int danoSom = 50;
const int consumoGrua = 300;
const int consumoToque = 150;
const int consumoSom = 50;
//curas
const int vidaRecuperadaCura1 = 100;
const int vidaRecuperadaCura2 = 200;
const int vidaRecuperadaCura3 = 400;
const int consumoCura1 = 200;
const int consumoCura2 = 300;
const int consumoCura3 = 400;
//atacantes
const int forcaTanque = 200;
const int forcaArtilharia = 500;
const int forcaInfantaria = 100;
const int ataquesTanque = 2;
const int ataquesArtilharia = 1;
const int ataquesInfantaria = 3;
const int vidaTanque = 200;
const int vidaArtilharia = 50;
const int vidaInfantaria = 100;

const int consumoTipoAtaque [] = {300,150,50};
const int danoTipoAtaque [] = {200,100,50};
//variaveis globais
float vidaBot;
float energiaBot;
int slot;
int turno;
int inimigosActivos;
int inimigosDerrotados;

//Memoria das posicoes

inimigo inimigos[nslots];

//Um Array que contem o tipo de ataque para cada slot
int ataques[nslots];

void limpaDisplay(){
   nxtDisplayCenteredTextLine(1, " ");
   nxtDisplayCenteredTextLine(2, " ");
   nxtDisplayCenteredTextLine(3, " ");
   nxtDisplayCenteredTextLine(4, " ");
   nxtDisplayCenteredTextLine(5, " ");
   nxtDisplayCenteredTextLine(6, " ");
  }



 void roda(){					//funcao que faz o robot rodar ate detectar cor
	motor[motorBase]=7;
	wait1Msec(750);
	while(SensorValue[sensorCor] == BLACKCOLOR)
		{
			  // continua a rodar
		}

	motor[motorBase]=0;

 }
void testeVidaInimigo(){
	limpaDisplay();
	nxtDisplayCenteredTextLine(5,"Energia: %.0f",energiaBot);
	//verifica se derrotou o inimigo
	nxtDisplayCenteredTextLine(1, "Inimigo a %.0f",inimigos[slot].vida);
	if(inimigos[slot].vida<=0){
		inimigos[slot].tipo=0;  //derrotado
	  	nxtDisplayCenteredTextLine(2, " ");
	  	nxtDisplayCenteredTextLine(3, "Derrotado!");
		inimigosDerrotados++;
		inimigosActivos--;
	}
		while(!SensorValue[sensorStart])
		{
			//espera pelo botao de start
		}
		limpaDisplay();
		nxtDisplayCenteredBigTextLine(3,"A ATACAR");


}
void empurrao(){
  PlaySoundFile("Hello.rso");
	motor[motorEmpurra]=-75;
	//PlaySound(soundBeepBeep);    // Play the sound, 'soundBeepBeep'.    // Play the sound file, 'Woops.rso'.
	wait1Msec(1000);
	motor[motorEmpurra]=0;
	energiaBot=energiaBot-consumoGrua;
	inimigos[slot].vida=inimigos[slot].vida-danoGrua;
	testeVidaInimigo();
}

void toque(){
    PlaySoundFile("Have A Nice Day.rso");
   motor[motorToque]=30;
	while(!SensorValue[sensorToque])    //  loop:
	{

	}
	motor[motorToque]=0;
	nMotorEncoderTarget[motorToque] = nMotorEncoder[motorToque]+10;
	motor[motorToque]=-30;
	while(nMotorRunState[motorToque] != runStateIdle)  //
	{
	  // ta a voltar o toque para cima
	}
	motor[motorToque] = 0;
	energiaBot=energiaBot-consumoToque;
	inimigos[slot].vida=inimigos[slot].vida-danoToque;
	testeVidaInimigo();
}

void som(){
	PlaySoundFile("Shout.rso");    // Ataque de som
	energiaBot=energiaBot-consumoSom;
	inimigos[slot].vida=inimigos[slot].vida-danoSom;
	testeVidaInimigo();
}
void voltaReconhecimento()
{
    PlaySoundFile("! Blips 07.rso");
	slot = 0;
	while(slot!=nslots)
	{
	  roda();

		switch(SensorValue[sensorCor]){

			case GREENCOLOR:
      PlaySoundFile("Green.rso");
				if((inimigos[slot].tipo)==0){     //se nao tinha nenhum atacante nesta posicao
				  nxtDisplayCenteredBigTextLine(3,"Tanque %d",slot);
				  wait1Msec(2000);
				  limpaDisplay();
				  nxtDisplayCenteredBigTextLine(3,"RECON");
				  inimigos[slot].tipo = 1;				//actualiza array com os valores predefinidos
				  inimigos[slot].forca = forcaTanque;
				  inimigos[slot].vida = vidaTanque;
				  inimigos[slot].ataques = ataquesTanque;
				  inimigos[slot].vidaInicial = vidaTanque;
				  inimigosActivos++;
				}

				break;
			case BLUECOLOR:
			PlaySoundFile("Blue.rso");
				nxtDisplayCenteredBigTextLine(3,"Artil %d",slot);
				if(inimigos[slot].tipo==0){     //se nao tinha nenhum atacante nesta posicao
					  wait1Msec(2000);
					  limpaDisplay();
					  nxtDisplayCenteredBigTextLine(3,"RECON");
					  inimigos[slot].tipo = 2;				//actualiza array com os valores predefinidos
					  inimigos[slot].forca = forcaArtilharia;
					  inimigos[slot].vida = vidaArtilharia;
					  inimigos[slot].ataques = ataquesArtilharia;
					  inimigos[slot].vidaInicial = vidaArtilharia;
					  inimigosActivos++;

				}

				break;
			case REDCOLOR:
			PlaySoundFile("Red.rso");
				nxtDisplayCenteredBigTextLine(3,"Infant %d",slot);
				if(inimigos[slot].tipo==0){     //se nao tinha nenhum atacante nesta posicao
					  wait1Msec(2000);
					  limpaDisplay();
					  nxtDisplayCenteredBigTextLine(3,"RECON");
					  inimigos[slot].tipo = 3;				//actualiza array com os valores predefinidos
					  inimigos[slot].forca = forcaInfantaria;
					  inimigos[slot].vida = vidaInfantaria;
					  inimigos[slot].ataques = ataquesInfantaria;
					  inimigos[slot].vidaInicial = vidaInfantaria;
					  inimigosActivos++;

				}

				break;
		 case WHITECOLOR:
		 PlaySoundFile("White.rso");
					slot++;
					nxtDisplayCenteredTextLine(5,"Slot %d",slot);
					wait1Msec(750);

			    break;

		}

	}

}
void recuperaEnergia(){
	energiaBot = energiaBot+(energiaBot*0.5);     //REGENERAR 50% da energia actual
	limpaDisplay();
	PlaySoundFile("Bravo.rso");
    nxtDisplayCenteredBigTextLine(3,"Regenerar");
    wait1Msec(1000);
	limpaDisplay();
    if (energiaBot >= energiaInicial){
     energiaBot=energiaInicial;

    }
	nxtDisplayCenteredTextLine(5,"Energia: %.0f",energiaBot);
	wait1Msec(1000);

 }


void testeVidaBot(){
  if(vidaBot<=0){
	limpaDisplay();
	PlaySoundFile("Ahnoo.rso");
	nxtDisplayCenteredBigTextLine(3,"X _ X");
	wait10Msec(1000);
	limpaDisplay();
	StopAllTasks();
	}
}

void sofreAtaque(){
int testealguem=0;
if(inimigosActivos>0){
PlaySoundFile("! Blips 07.rso");
	for(int i=0;i<nslots;i++){
		if(inimigos[i].tipo != 0 && inimigos[i].ataques > 0){
			testealguem = 1;
		}
	}
	}
	if(testealguem==1){
		float aux;
		slot=0;
		 nxtDisplayCenteredBigTextLine(2,"Taking");
		 nxtDisplayCenteredBigTextLine(4,"Fire");
		while(slot!=nslots)	{
			roda();
			if(SensorValue[sensorCor]==WHITECOLOR){
			  PlaySoundFile("White.rso");
				slot++;
				nxtDisplayCenteredTextLine(6,"Slot %d",slot);
				wait1Msec(750);
			}
			else{
				if(inimigos[slot].tipo != 0){
				   //o atacante na posicao i vai atacar se ainda tiver ataques
					if(inimigos[slot].ataques>0){
					  aux = (inimigos[slot].vida/inimigos[slot].vidaInicial)*inimigos[slot].forca;
					  PlaySoundFile("Buuuhh.rso");
					  vidaBot=vidaBot-aux; //robot perde vida devido ao ataque
					  testeVidaBot();
					  inimigos[slot].ataques--;
					  if(inimigos[slot].ataques==0){
					    inimigosActivos--;
					    inimigosDerrotados++;
					  }
					 //falta actualizar o display e som do ataque
					  limpaDisplay();
					  nxtDisplayCenteredTextLine(2,"Sofreu ataque");
					  nxtDisplayCenteredTextLine(4,"Vida bot :%.0f",vidaBot);
					  wait1Msec(3000);
					  limpaDisplay();
					  nxtDisplayCenteredBigTextLine(2,"Taking");
					  nxtDisplayCenteredBigTextLine(4,"Fire");
					}
				 }
			 }
			 }

		    nxtDisplayCenteredTextLine(1, "Sofreu");
			nxtDisplayCenteredTextLine(2, "todos os");
			nxtDisplayCenteredTextLine(3, "ataques");
			wait1Msec(2000);
		}

}

void voltaAtaque(){
if(inimigosActivos>0){
    PlaySoundFile("Aha.rso");
	  nxtDisplayCenteredBigTextLine(3,"A Atacar");
		slot=0;
		while(slot!=nslots)	{

			roda();
			if(SensorValue[sensorCor]==WHITECOLOR){
			  PlaySoundFile("White.rso");
				slot++;
				nxtDisplayCenteredTextLine(5,"Slot %d",slot);
				wait1Msec(750);
			}
			else{
				if(ataques[slot] != 0){
					switch(ataques[slot]){
						case 1:
							if(energiaBot>=300){
							empurrao();
							}
						break;
						case 2:
						if(energiaBot>=150){
							toque();
							}
						break;
						case 3:
						if(energiaBot>=50){
							som();
							}
						break;
					}
				}
			}
		}
	}
}
void decideAtaque(int energiaMin){
 // wait10Msec(500);
  int p = 1;
  int consome=0;
  int nMaxAtaques=inimigosActivos;
    //ciclo dos inimigos
  while(p<=nMaxAtaques){
	  for(int i=0;i<nslots;i++){
	    //ciclo dos ataques
		  if(inimigos[i].prioridade==p){
		    for(int a=0;a<3;a++){
		           if(energiaBot-consumoTipoAtaque[a] >=energiaMin && danoTipoAtaque[a]<=inimigos[i].vida) {

		              consome=consome+consumoTipoAtaque[a];
		              if(p<nMaxAtaques){
		                 if(energiaBot-consome-((nMaxAtaques-p)*danoSom)>=energiaMin){ //energiaBot-consome>=energiaMin &&
							          ataques[i]=a+1;
							          break;
		                 }
		                 else{
		                   consome=consome-consumoTipoAtaque[a];
		                   }
		                }
		               else{

		                  if(energiaBot-consome>=energiaMin){
							          ataques[i]=a+1;
							          break;
		                 }
		                 else{
		                   consome=consome-consumoTipoAtaque[a];
		                   }

		                }
	            }

	        }
	      i=0;
	      p++;
	   }
    }
  }

}
void pensa(){
  int p=1;
  int i=0;
  int tanques=0;
	//inicializa o array dos ataques
	for(int i = 0; i<nslots;i++){
		ataques[i]=0;
		inimigos[i].prioridade=0;
	}
   nxtDisplayCenteredBigTextLine(3,"A Pensar");
   //caso geral para artilharia(ataca sempre com som)
 while(i<nslots){
    if(inimigos[i].tipo==2){
      if(inimigos[i].ataques>0){
      inimigos[i].prioridade=p;
      p++;
    }
  }
    i++;
  }
  i=0;
	while(i<nslots){
    if(inimigos[i].tipo==3){
       if(inimigos[i].ataques>0){
      inimigos[i].prioridade=p;
      p++;
    }
  }
    i++;
  }
  i=0;
  while(i<nslots){
    if(inimigos[i].tipo==1){
       if(inimigos[i].ataques>0){
      inimigos[i].prioridade=p;
      p++;
        tanques++;
    }}
    i++;

  }
  //caso especial se vier 3 tranques
  if(tanques==3 && inimigosActivos==3){
    decideAtaque(225);
  }
  else if(inimigosActivos+inimigosDerrotados<4){
   //deixar 300
    decideAtaque(300);
  }
  else if(inimigosActivos+inimigosDerrotados==4){
    //deixar 200
  decideAtaque(200);
  }
  else if(inimigosActivos+inimigosDerrotados>=5){
    //deixar 0
  decideAtaque(0);
  }
  PlaySoundFile("Brilliant.rso");
	wait1Msec(800);
}
void fimJogo(){
limpaDisplay();
  PlaySoundFile("Good Job.rso");
	nxtDisplayCenteredBigTextLine(3,"^^;");

	 while(!SensorValue[sensorStart])
		{
			//espera pelo botao de start
		}
		StopAllTasks();
}
task main()
{
  PlaySoundFile("Hello.rso");
  wait1Msec(1000);
  //Inicializacoes
	inimigosActivos = 0;
	inimigosDerrotados = 0;
	turno=0;
	vidaBot=vidaInicial;
	energiaBot=energiaInicial;
	for(int i = 0; i<nslots;i++){		//inicializar o array dos inimigos
	  inimigos[i].tipo=0;
	}
	//JOGO
	nxtDisplayCenteredTextLine(1, "Prima");
	nxtDisplayCenteredBigTextLine(3,"START");
	PlaySoundFile("Start.rso");
	while(!SensorValue[sensorStart])
		{
			//espera pelo botao de start
		}
	for(int i = 0; i<turnos;i++){
	  turno = i;
		limpaDisplay();
		nxtDisplayCenteredTextLine(1, "Em");
		nxtDisplayCenteredBigTextLine(3,"RECON");
		nxtDisplayCenteredTextLine(6,"Turno %d",turno+1);
		voltaReconhecimento();
		pensa();
		voltaAtaque();
		sofreAtaque();
		recuperaEnergia();
		nxtDisplayCenteredTextLine(1, "Fim de turno :%d",turno+1);
		nxtDisplayCenteredBigTextLine(3,"START?");
		nxtDisplayCenteredTextLine(6,"Vida: %.0f",vidaBot);
		PlaySoundFile("Start.rso");
		 while(!SensorValue[sensorStart])
		{
			//espera pelo botao de start
		}
		wait1Msec(1000);
	}

  fimJogo();


}

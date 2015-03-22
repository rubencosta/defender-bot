
//constantes
//ambiente
const turnos = 6;
const nslots = 6;
//robot
var vidaInicial = prompt("Quantidade de Vida Inicial?",750);
var energiaInicial = prompt("Quantidade de Energia Inicial?",500);
//ataques
const danoGrua = 200;
const danoToque = 100;
const danoSom = 50;
const consumoGrua = 300;
const consumoToque = 150;
const consumoSom = 50;
//curas
const vidaRecuperadaCura1 = 100;
const vidaRecuperadaCura2 = 200;
const vidaRecuperadaCura3 = 400;
const consumoCura1 = 200;
const consumoCura2 = 300;
const consumoCura3 = 400;
//atacantes
const forcaTanque = 200;
const forcaArtilharia = 500;
const forcaInfantaria = 100;
const ataquesTanque = 2;
const ataquesArtilharia = 1;
const ataquesInfantaria = 3;
const vidaTanque = 200;
const vidaArtilharia = 50;
const vidaInfantaria = 100;
const consumoTipoAtaque = [300,150,50];
const danoTipoAtaque = [200,100,50];
//variaveis globais
var vidaBot;
var energiaBot;
var turno;
var inimigos = new Array;
var tabuleiro = new Array;
var ataques = new Array;
var vitorias;
var jogos;
var percentagemVitorias;
var energiaMedia;
var vidaMedia;
$.ui.ready(
    function () {
        $.ui.disableSideMenu();//disable the side menu on load
        vitorias=0;
        jogos=0;
        percentagemVitorias=0;energiaMedia=0;
        vidaMedia=0;
    }
);

simular = function (){
    var simuls = prompt("Quantidade de Simulações",1000);
    $.ui.showMask();
    for(var i=0;i<simuls;i++){
        jogo();
    }
    $.ui.hideMask();
    return;
}

jogo = function (){
        inicializa();
        for(var i = 1; i<=turnos;i++){
          turno = i;
           posiciona();
		 	pensa();
		  voltaAtaque();
		  sofreAtaque();
		  energiaBot+=energiaBot/2;
        if(energiaBot>energiaInicial){ 
            energiaBot=energiaInicial;
             }
	   }

  fimJogo();
    return;
}

fimJogo = function(){
   jogos++;
   if(vidaBot>0){ 
           vitorias++;
           percentagemVitorias=(vitorias/jogos)*100;
           energiaMedia = ((energiaMedia*(vitorias-1))+energiaBot)/vitorias;
           vidaMedia = ((vidaMedia*(vitorias-1))+vidaBot)/vitorias;
           
           }
   else{
       console.log("PERDEU")
       for(var i=0;i<nslots;i++){
           console.log(inimigos[i]);
           console.log(tabuleiro[i]);
       }
   }
   $("#afui #content #tabela-info #percentagemVitorias").html(percentagemVitorias);
   $("#afui #content #tabela-info #vitorias").html(vitorias);
   $("#afui #content #tabela-info #jogos").html(jogos);
   $("#afui #content #tabela-info #vidaMedia").html(vidaMedia);
   $("#afui #content #tabela-info #energiaMedia").html(energiaMedia);
   return;
}
inicializa=function(){
    vidaBot=vidaInicial;
    energiaBot=energiaInicial;
    turno=0;
    var tipo;
    for(var i=0;i<6;i++){ 
        tipo = Math.floor((Math.random()*3)+1);
        inimigos[i]= new inimigo(tipo);
        tabuleiro[i] = new Object();
        tabuleiro[i].tipo=tipo;
        tabuleiro[i].turno=Math.floor((Math.random()*6)+1);
       
    }
    for(var i=0;i<6;i++){ 
        console.log(tabuleiro[i].tipo);
        console.log(tabuleiro[i].turno);
       
    }
    ////////////////////////////////////
    inimigosActivos = 0;
	inimigosDerrotados = 0;
  ////////////////////////  

return;
}

posiciona = function(){
    for(var i=0;i<nslots;i++){
         console.log(tabuleiro[i].turno);
      if(tabuleiro[i].turno==turno){
          inimigos[i].colocado=true;
          inimigosActivos++;
           }
        }
    return;
}

sofreAtaque =function(){
var aux;
  for(var i=0; i<6;i++){
    if(inimigos[i].colocado){
       //o atacante na posicao i vai atacar se ainda tiver ataques
        if(inimigos[i].ataques>0){
          aux = inimigos[i].vida/inimigos[i].vidaInicial; //vai calcular a percentagem de vida do inimigo
          vidaBot-=inimigos[i].forca*aux; //robot perde vida devido ao ataque
          inimigos[i].ataques--;
           if(inimigos[i].ataques==0){
                inimigosActivos--;
                inimigosDerrotados++;    
           }  
        }
     }
  }
    return;
}

voltaAtaque = function (){
if(inimigosActivos>0){
        for(var i=0;i<nslots;i++){
				if(ataques[i] != 0){
					switch(ataques[i]){
						case 1:
							if(energiaBot>=300){
							grua(i);
							}
						break;
						case 2:
						if(energiaBot>=150){
							toque(i);
							}
						break;
						case 3:
						if(energiaBot>=50){
							som(i);
							}
						break;
					}
				}
            }
		}
    return;
}
decideAtaque=function (energiaMin){
 // wait10Msec(500);
  var p = 1;
  var a ;
  var para=false;
  var consome=0;
  var nMaxAtaques=inimigosActivos;
  //ciclo dos inimigos
  while(p<=nMaxAtaques){
	  for(var i=0;i<nslots;i++){
	      //ciclo dos ataques
		  if(inimigos[i].prioridade==p){
              a=0;
              para=false;
		  while(a<3 && !para){ 
              
		           if(energiaBot-consumoTipoAtaque[a] >=energiaMin && danoTipoAtaque[a]<=inimigos[i].vida) {

		              consome=consome+consumoTipoAtaque[a];
		              if(p<nMaxAtaques){
		                 if(energiaBot-consome-((nMaxAtaques-p)*danoSom)>=energiaMin){ //energiaBot-consome>=energiaMin &&
							          ataques[i]=a+1;
							          para=true;
		                 }
		                 else{
		                   consome=consome-consumoTipoAtaque[a];
		                   }
		                }
		               else{

		                  if(energiaBot-consome>=energiaMin){
							          ataques[i]=a+1;
							          para=true;
		                 }
		                 else{
		                   consome=consome-consumoTipoAtaque[a];
		                   }

		                }
	            }
              a++;
	        }
	      i=0;
	      p++;
	   }
    }
  }
    return;
}
pensa= function (){
    if(inimigosActivos>0){
          var p=1;
          var i=0;
          var tanques=0;
            //inicializa o array dos ataques
            for(var j = 0; j<nslots;j++){
                ataques[j]=0;
                inimigos[j].prioridade=0;
            }
           //nxtDisplayCenteredBigTextLine(3,"A Pensar");
           //caso geral para artilharia(ataca sempre com som)
         while(i<nslots){
            if(inimigos[i].tipo==2 && inimigos[i].colocado){
              if(inimigos[i].ataques>0){
              inimigos[i].prioridade=p;
              p++;
            }
          }
            i++;
          }
          i=0;
            while(i<nslots){
            if(inimigos[i].tipo==3 && inimigos[i].colocado){
               if(inimigos[i].ataques>0){
              inimigos[i].prioridade=p;
              p++;
            }
          }
            i++;
          }
          i=0;
          while(i<nslots){
            if(inimigos[i].tipo==1 && inimigos[i].colocado){
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
            //wait1Msec(800);
    }
    return;
}
function inimigo(tipo)
{
    switch(tipo){
        case 1:
            //tanque
            this.name="Tanque";
            this.vidaInicial=vidaTanque;
            this.forca=forcaTanque;
            this.ataques=ataquesTanque;
            this.vida=vidaTanque;
            this.prioridade=0;
            break;
        case 2:
            //Artilharia
            this.name="Artilharia";
            this.vidaInicial=vidaArtilharia;
            this.forca=forcaArtilharia;
            this.ataques=ataquesArtilharia;
            this.vida=vidaArtilharia;
            this.prioridade=0;
            break;
        case 3:
            //Infantaria
            this.name="Infantaria";
            this.vidaInicial=vidaInfantaria;
            this.forca=forcaInfantaria;
            this.ataques=ataquesInfantaria;
            this.vida=vidaInfantaria;
            this.prioridade=0;
            break;
    }
    this.tipo=tipo;
    this.colocado=false;
}


grua=function(slot){
   
        energiaBot-=consumoGrua;
        inimigos[slot].vida-=danoGrua;
        if(inimigos[slot].vida<=0){
            inimigos[slot].colocado=false;
            inimigosDerrotados++;
		    inimigosActivos--;
        }
   
    return;
}
toque=function(slot){
     
        energiaBot-=consumoToque;
        inimigos[slot].vida-=danoToque;
        if(inimigos[slot].vida<=0){
            inimigos[slot].colocado=false;
            inimigosDerrotados++;
		    inimigosActivos--;
        }
  
    return;
}
som=function(slot){
    
        energiaBot-=consumoSom;
        inimigos[slot].vida-=danoSom;
        if(inimigos[slot].vida<=0){
            inimigos[slot].colocado=false;
            inimigosDerrotados++;
		    inimigosActivos--;
        }
  
    return;
}
cura1=function(){
     if(energiaBot>=consumoCura1){
        energiaBot-=consumoCura1;
        vidaBot+=vidaRecuperadaCura1;
         if(vidaBot>vidaInicial)
             vidaBot=vidaInicial;
    }
   
    return;
}
cura2=function(){
     if(energiaBot>=consumoCura2){
        energiaBot-=consumoCura2;
        vidaBot+=vidaRecuperadaCura2;
         if(vidaBot>vidaInicial)
             vidaBot=vidaInicial;
    }
   
    return;
}
cura3=function(){
     if(energiaBot>=consumoCura3){
        energiaBot-=consumoCura3;
        vidaBot+=vidaRecuperadaCura3; 
         if(vidaBot>vidaInicial)
             vidaBot=vidaInicial;
    }
    return;
}

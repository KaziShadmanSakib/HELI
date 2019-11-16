#include<stdio.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define SCREEN_WIDTH 1500 
#define SCREEN_HEIGHT 1000
#define SPEED 350 
#define FPS 60 
#define numberofbullets 20
#define bulletdelay 1000

int score;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* menu_surface = NULL;
TTF_Font *font = NULL;
SDL_Surface* bullet_img = NULL;
SDL_Texture*  bullet_tex = NULL;
SDL_Surface* life_img = NULL;
SDL_Texture* life_tex = NULL;
SDL_Surface* player = NULL;
SDL_Texture*  player_tex = NULL;
SDL_Surface* obstacle = NULL;
SDL_Texture*  obstacle_tex = NULL;
SDL_Surface* background = NULL;
SDL_Texture*  background_tex = NULL;
SDL_Surface* load = NULL;
SDL_Texture*  load_tex = NULL;
SDL_Surface* menu = NULL;
SDL_Texture*  menu_tex = NULL;
SDL_Surface* highscore_img = NULL;
SDL_Texture* highscore_tex = NULL;
SDL_Surface* credit = NULL;
SDL_Texture*  credit_tex = NULL;
SDL_Surface* instruction = NULL;
SDL_Texture* instruction_tex = NULL;
SDL_Surface* pre_menu = NULL;
SDL_Texture*  pre_menu_tex = NULL;
SDL_Surface* gameover_img = NULL;
SDL_Texture* gameover_tex = NULL;

int menumenu(){
	/*Loading the Menu Image into memory using SDL Image Library Function*/
	menu = SDL_LoadBMP("Images/menu.bmp");
	if(menu == NULL){
		printf("Error Creating Menu\n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	/*Loading the Menu Image Data Into the Graphics Hardware's Memory*/
	menu_tex = SDL_CreateTextureFromSurface(renderer,menu);
	SDL_FreeSurface(menu);
	if(menu_tex == NULL){
		printf("Error Creating Menu Texture: %s\n",SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	SDL_RenderCopy(renderer,menu_tex,NULL,NULL);

	/*Menu Text Type*/
	{
		font  = TTF_OpenFont("Images/con.ttf",1000);	
		SDL_Texture *text[5];
		SDL_Color text_color = {255,255,255,255};
		SDL_FreeSurface(menu_surface);
		menu_surface = TTF_RenderText_Solid(font,"NEW GAME",text_color);
		text[0] = SDL_CreateTextureFromSurface(renderer,menu_surface);
		SDL_FreeSurface(menu_surface);
		menu_surface = TTF_RenderText_Solid(font,"HIGHSCORE",text_color);
		text[1] = SDL_CreateTextureFromSurface(renderer,menu_surface);
		SDL_FreeSurface(menu_surface);
		menu_surface = TTF_RenderText_Solid(font,"INSTRUCTIONS",text_color);
		text[2] = SDL_CreateTextureFromSurface(renderer,menu_surface);
		SDL_FreeSurface(menu_surface);
		menu_surface = TTF_RenderText_Solid(font,"CREDIT",text_color);
		text[3] = SDL_CreateTextureFromSurface(renderer,menu_surface);
		menu_surface = TTF_RenderText_Solid(font,"QUIT",text_color);
		text[4] = SDL_CreateTextureFromSurface(renderer,menu_surface);
		SDL_Rect text_position;
		int i,j;
		for(i=0,j=0;i<5;i++,j+=200){
			text_position.x = 1000;
			text_position.y = -50+j;
			text_position.w = 300;
			text_position.h = 250;
			SDL_RenderCopy(renderer,text[i],NULL,&text_position);
		}
		SDL_RenderPresent(renderer);

		/*Menu Event*/
		SDL_Event e;
		bool menu_run = true;
		while(menu_run){
			if(SDL_PollEvent(&e)){
				switch(e.type){
					case SDL_QUIT :
						menu_run =false;
						break;
					case SDL_MOUSEBUTTONDOWN:
					{
						if(e.motion.x>=1000&&e.motion.x<=1300&&e.motion.y>=-50&&e.motion.y<200){
							return 0;
							break;
						}
						if(e.motion.x>=1000&&e.motion.x<=1300&&e.motion.y>=200&&e.motion.y<400){
							return 1;
							break;
						}
						if(e.motion.x>=1000&&e.motion.x<=1300&&e.motion.y>=400&&e.motion.y<650){
							return 2;
							break;
						}
						if(e.motion.x>=1000&&e.motion.x<=1300&&e.motion.y>=650&&e.motion.y<=900){
							return 3;
							break;
						}
						if(e.motion.x>=1000&&e.motion.x<=1300&&e.motion.y>=900&&e.motion.y<=1150){
							return 4;
							break;
						}
					}
					default:
					break;
				}
			}
		} 
	}
}
/*Things for Obstacle*/
int spawn[SCREEN_WIDTH];

/*Rect for Obstacle Position*/
SDL_Rect destO[SCREEN_WIDTH];

/*Player Lives*/
int lives = 3;

/*Bullet*/
class bullet{
	public:
        void init();        
        void render();
        void destroy();
        SDL_Rect bullet;

    private:
        int xpos , ypos;
        
};

void bullet::render(){
	bullet.h= 10;
	bullet.w= 5;
	bullet.y -= 6;

	/*Loading the Bullet image into memory using SDL Image Library Function*/
	bullet_img = SDL_LoadBMP("Images/bullet.bmp");
	if(bullet_img == NULL){
		printf("Error Creating Player\n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	/*Loading the Bullet Image Data Into the Graphics Hardware's Memory*/
	bullet_tex = SDL_CreateTextureFromSurface(renderer,bullet_img);
	SDL_FreeSurface(bullet_img);
	if(bullet_tex == NULL){
		printf("Error Creating Bullet Texture: %s\n",SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	/*Draw the Bullet to the Window*/
 	SDL_RenderCopy(renderer,bullet_tex,NULL,&bullet);
}


/*Making Random Obstacle*/
int random_obstacle(int x, int rand ,SDL_Rect p ,bullet B[]){
	if(rand==2){
		spawn[x]=1;
	    destO[x].x = x;
	}
	for(int i=0;i<(SCREEN_WIDTH-50);i++){
		if(spawn[i]==1){
			SDL_RenderCopy(renderer,obstacle_tex,NULL,&destO[i]);
	    	destO[i].y+=2.5;
	    //obstacle gone
	    if(destO[i].y>=SCREEN_HEIGHT){
	       spawn[i]=0;
	       destO[i].y=0;
	       lives--;
	       printf("Life: %d\n" ,lives);
	    }
	    if(p.y>=(destO[i].y-p.h) && p.y<=(destO[i].y+destO[i].h)){
	    if(p.x<=(destO[i].x+destO[i].w) && p.x>=destO[i].x){spawn[i]=0;destO[i].y=0;lives--;printf("Life: %d\n" ,lives);}
	    if((p.x+p.w)<=(destO[i].x+destO[i].w) && (p.x+p.w)>=destO[i].x){spawn[i]=0;destO[i].y=0;lives--;printf("Life: %d\n" ,lives);}
	    }
        if(p.x>=(destO[i].x+p.x) && p.x<=(destO[i].x+destO[i].w)){
	    if(p.y<=(destO[i].y+destO[i].h) && p.y>=destO[i].y){spawn[i]=0;destO[i].y=0;lives--;printf("Life: %d" ,lives);}
	    if((p.y+p.h)<=(destO[i].y+destO[i].h) && (p.y+p.h)>=destO[i].y){spawn[i]=0;destO[i].y=0;lives--;printf("Life: %d\n" ,lives);}
	    }
	    for(int j=0;j<numberofbullets;j++){
            if(B[j].bullet.x>=destO[i].x && (B[j].bullet.x+B[j].bullet.w)<=(destO[i].x+destO[i].w)){
                if(B[j].bullet.y<=(destO[i].y+destO[i].h)){
                    spawn[i] = 0;
                    B[j].bullet.x = SCREEN_WIDTH+100;
                    B[j].bullet.y = SCREEN_HEIGHT+1000000;
                }
            }
	    }
      }
	}
}

/*Update HighScore*/
void highscore(int t){
	FILE *fp;
	fp = fopen("highscore.txt","r");
	int a[5];
	int i=0;
	while(fscanf(fp,"%d",&a[i])==1){
		i++;
		if(i==5)  break;
	}
	if(t>a[4])  a[4]=t;
	for(i=0;i<4;i++){
		for(int j=i+1;j<5;j++){
			if(a[i]<a[j]){
				int tmp=a[i];
				a[i]=a[j];
				a[j]=tmp;
		}
	}
}
	FILE *fw=fopen("highscore.txt","w");
	fprintf(fw,"%d\n%d\n%d\n%d\n%d\n",a[0],a[1],a[2],a[3],a[4]);
	fclose(fp);
	fclose(fw);
}

/*Function for Optimizing*/
void fps_optimize(Uint32 starting_tick){
	if((1000/FPS)>SDL_GetTicks()-starting_tick){
		SDL_Delay(1000/FPS-(SDL_GetTicks()-starting_tick));
	}
}

int main(int argc,char* args[])
{	
	int currentscore;
	/*Spawn Obstacles Randomly*/
	for(int i=0;i<(SCREEN_WIDTH-50);i++){
		destO[i].w=50;
		destO[i].h=60;
	}
	memset(spawn,0,sizeof(spawn[0])*1000);
	/*Initializing Graphics and Timer System*/
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0){
		printf("Error Initializing SDL: %s\n",SDL_GetError());
		return 1;
	}

	if(TTF_Init()==-1){
    	printf("Error Initializing TTF: %s\n", TTF_GetError());
    	return 1;
	}

	/*Creating a Window*/
	window = SDL_CreateWindow("HELI",
							   SDL_WINDOWPOS_CENTERED,
							   SDL_WINDOWPOS_CENTERED,
							   SCREEN_WIDTH,SCREEN_HEIGHT,
							   SDL_WINDOW_RESIZABLE);
	if(window == NULL){
		printf("Error Creating Window: %s\n",SDL_GetError());
		SDL_Quit();
		return 1;
	}

	/*Creating a Renderer, Which Sets Up the Graphics Hardware*/
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;
	renderer = SDL_CreateRenderer(window,-1,render_flags);
	if(renderer == NULL){
		printf("Error Creating Renderer: %s\n",SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	/*Loading the Pre_Menu image into memory using SDL Image Library Function*/
	pre_menu = SDL_LoadBMP("Images/pre_menu.bmp");
	/*Loading the Pre_Menu Image Data Into the Graphics Hardware's Memory*/
	pre_menu_tex = SDL_CreateTextureFromSurface(renderer,pre_menu);
	SDL_FreeSurface(pre_menu);
	SDL_RenderCopy(renderer,pre_menu_tex,NULL,NULL);
	SDL_RenderPresent(renderer);
	SDL_Delay(1500);

	/*Loading the Background image into memory using SDL Image Library Function*/
	background = SDL_LoadBMP("Images/background.bmp");
	if(background == NULL){
		printf("Error Creating background\n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	/*Loading the Background Image Data Into the Graphics Hardware's Memory*/
	background_tex = SDL_CreateTextureFromSurface(renderer,background);
	SDL_FreeSurface(background);
	if(background_tex == NULL){
		printf("Error Creating Background Texture: %s\n",SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}	

	/*Loading the Player image into memory using SDL Image Library Function*/
	player = SDL_LoadBMP("Images/player.bmp");
	if(player == NULL){
		printf("Error Creating Player\n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	/*Loading the Player Image Data Into the Graphics Hardware's Memory*/
	player_tex = SDL_CreateTextureFromSurface(renderer,player);
	SDL_FreeSurface(player);
	if(player_tex == NULL){
		printf("Error Creating Player Texture: %s\n",SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	/*Loading the Obstacle image into memory using SDL Image Library Function*/
	obstacle = SDL_LoadBMP("Images/obstacle.bmp");
	if(obstacle == NULL){
		printf("Error Creating obstacle\n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	/*Loading the Obstacle Image Data Into the Graphics Hardware's Memory*/
	obstacle_tex = SDL_CreateTextureFromSurface(renderer,obstacle);
	SDL_FreeSurface(obstacle);
	if(obstacle_tex == NULL){
		printf("Error Creating obstacle Texture: %s\n",SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	/*Structure to Hold the Position and Size of the Player*/
	SDL_Rect dest;

	/*Get the Dimensions of Player*/
	SDL_QueryTexture(player_tex,NULL,NULL,&dest.w,&dest.h);
	dest.w;
	dest.h;

 	/* Start the Player in Center of Screen*/
 	float x_pos = (SCREEN_WIDTH - dest.w)/2;
 	float y_pos = (SCREEN_HEIGHT - dest.h);
 	float x_vel = 0;
 	float y_vel = 0;

 	/*Keep Track of Which Inputs are Given*/
 	int up = 0;
 	int down = 0;
 	int left = 0;
 	int right = 0;

 	here:
 	int menu_menu = menumenu();
 	printf("%d\n",menu_menu);

 	/*Animation Loop*/
 	Uint32 starting_tick;
 	if(menu_menu==0){
  		printf("Life: %d\n" ,lives);
 		
 		SDL_Rect life;
 		
 		/*Loading the Loading image into memory using SDL Image Library Function*/
		load = SDL_LoadBMP("Images/loading.bmp");
		/*Loading the Loading Image Data Into the Graphics Hardware's Memory*/
		load_tex = SDL_CreateTextureFromSurface(renderer,load);
		SDL_FreeSurface(load);
		SDL_RenderCopy(renderer,load_tex,NULL,NULL);
		SDL_RenderPresent(renderer);
		SDL_Delay(2000);

		/*Playing Audio*/
 		Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1048);
		Mix_Music*backgroundSound=Mix_LoadMUS("Audio/gamesong.mp3");
		Mix_PlayMusic(backgroundSound, -1);
		
 		bullet B[numberofbullets];
 		int initial = 0;
 		int time=0;
 		
 		SDL_Event event;
 		bool running = true;
 		while(running){
 			if(lives==0){
 			/*Loading the Instruction image into memory using SDL Image Library Function*/
			gameover_img = SDL_LoadBMP("Images/gameover.bmp");
			/*Loading the Instruction Image Data Into the Graphics Hardware's Memory*/
			gameover_tex = SDL_CreateTextureFromSurface(renderer,gameover_img);
			SDL_FreeSurface(gameover_img);
			SDL_RenderCopy(renderer,gameover_tex,NULL,NULL);
			SDL_RenderPresent(renderer);
			SDL_Delay(2000);
			goto here;
 			}
 			currentscore = SDL_GetTicks()/1000;
 			while(SDL_PollEvent(&event)){
 				/*Initializing Bullet*/
 				if(event.key.keysym.scancode==SDL_SCANCODE_SPACE && event.key.repeat == 0){
 					time++;
 					if(time>1){
 					initial++;
 					if(initial>=numberofbullets){ 
 						initial=0;
 					}
 					printf("Bullets till reset = %d\n", numberofbullets-initial);
 					B[initial].bullet.x = x_pos + dest.w/2 -2;
 					B[initial].bullet.y = y_pos;
 					time =0;
 					}
 				}
 				switch(event.type){
 					case SDL_QUIT:
 						running = false;
 						break;
 					case SDL_KEYDOWN:
 						switch(event.key.keysym.scancode){
 							case SDL_SCANCODE_W:
 							case SDL_SCANCODE_UP:
 								up = 1;
 								break;
 							case SDL_SCANCODE_A:
 							case SDL_SCANCODE_LEFT:
 								left = 1;
 								break;
 							case SDL_SCANCODE_S:
 							case SDL_SCANCODE_DOWN:
 								down = 1;
 								break;
 							case SDL_SCANCODE_D:
 							case SDL_SCANCODE_RIGHT:
 						 		right = 1;
 						 		break;
 						}
 						break; 
 					case SDL_KEYUP:
 						switch(event.key.keysym.scancode){
 							case SDL_SCANCODE_W:
 							case SDL_SCANCODE_UP:
 								up = 0;
 								break;
 							case SDL_SCANCODE_A:
 							case SDL_SCANCODE_LEFT:
 								left = 0;
 								break;
 							case SDL_SCANCODE_S:
 							case SDL_SCANCODE_DOWN:
 								down = 0;
 								break;
 							case SDL_SCANCODE_D:
 							case SDL_SCANCODE_RIGHT:
 						 		right = 0;
 						 		break;
 					}
 					break;
 				}
 			}
 			/*Determine the Velocity*/
 			x_vel = 0;
 			y_vel = 0;
 			if(up && !down) y_vel = -SPEED;
 			if(down && !up) y_vel = SPEED;
 			if(left && !right) x_vel = -SPEED;
 			if(right && !left) x_vel = SPEED;

 			/*Update Positions*/
 			x_pos += x_vel/60;
 			y_pos +=y_vel/60;

 			/*Collision Detection with Borders*/
 			if(x_pos<=0) x_pos = 0;
 			if(y_pos<=0) y_pos = 0;
 			if(x_pos>=SCREEN_WIDTH - dest.w) x_pos = SCREEN_WIDTH - dest.w;
 			if(y_pos>=SCREEN_HEIGHT - dest.h) y_pos = SCREEN_HEIGHT - dest.h;

 			/*Set the Positions in the Structure*/
 			dest.y = (int) y_pos;
 			dest.x = (int) x_pos;

 			/*Clear the Window*/
 			SDL_RenderClear(renderer);

 			/*Draw the Background to the Window*/
 			SDL_RenderCopy(renderer,background_tex,NULL,NULL);


 			/*Draw the Bullet*/
 			for(int start=0;start<=initial;start++){
 				if(B[start].bullet.y>0)
 				B[start].render();
 			}

 			/*Draw the Player to the Window*/
 			SDL_RenderCopy(renderer,player_tex,NULL,&dest);

 			/*Draw the Obstacle*/
 			int x=rand()%(SCREEN_WIDTH-50);
 			int p=rand()%60;
            random_obstacle(x ,p ,dest,B);


 			SDL_Delay(1000/FPS);

 			if(lives>0)	{
 				life.x = 100;
 				life.y = 50;
 				life.w = 50;
 				life.h = 50;
				life_img = SDL_LoadBMP("Images/life.bmp");
				life_tex = SDL_CreateTextureFromSurface(renderer,life_img);
				SDL_FreeSurface(life_img);
				SDL_RenderCopy(renderer,life_tex,NULL,&life);      
 			}
 			if(lives>1){
 				life.x = 160;
		     	life_img = SDL_LoadBMP("Images/life.bmp");
				life_tex = SDL_CreateTextureFromSurface(renderer,life_img);
				SDL_FreeSurface(life_img);
				SDL_RenderCopy(renderer,life_tex,NULL,&life); 
 			}
 			if(lives>2){
 				life.x = 220;
		    	life_img = SDL_LoadBMP("Images/life.bmp");
				life_tex = SDL_CreateTextureFromSurface(renderer,life_img);
				SDL_FreeSurface(life_img);
				SDL_RenderCopy(renderer,life_tex,NULL,&life); 
 			}
 			SDL_RenderPresent(renderer);

 			/*Using the Optimizing Function*/
 			fps_optimize(starting_tick);
 		}
 	}

 	if(lives==0){
 		printf("Score : %d\n", currentscore);
 	}

 	if(menu_menu==1){
  		/*Loading the Instruction image into memory using SDL Image Library Function*/
		highscore_img = SDL_LoadBMP("Images/highscore.bmp");
		/*Loading the Instruction Image Data Into the Graphics Hardware's Memory*/
		highscore_tex = SDL_CreateTextureFromSurface(renderer,highscore_img);
		SDL_FreeSurface(highscore_img);
		SDL_Event event2;
		bool running = true;
 		while(running){
 			while(SDL_PollEvent(&event2)){
 				if(event2.key.keysym.scancode==SDL_SCANCODE_ESCAPE){	
 					goto here;
 				}
 				switch(event2.type){
 					case SDL_QUIT:
 						running = false;
 						break;
 				}
 			}
 			SDL_RenderCopy(renderer,highscore_tex,NULL,NULL);
			SDL_RenderPresent(renderer);
 		}	
 	}
 	
 	if(menu_menu==2){
  		/*Loading the Instruction image into memory using SDL Image Library Function*/
		instruction = SDL_LoadBMP("Images/instruction.bmp");
		/*Loading the Instruction Image Data Into the Graphics Hardware's Memory*/
		instruction_tex = SDL_CreateTextureFromSurface(renderer,instruction);
		SDL_FreeSurface(instruction);
		SDL_Event event2;
		bool running = true;
 		while(running){
 			while(SDL_PollEvent(&event2)){
 				if(event2.key.keysym.scancode==SDL_SCANCODE_ESCAPE){	
 					goto here;
 				}
 				switch(event2.type){
 					case SDL_QUIT:
 						running = false;
 						break;
 				}
 			}
 			SDL_RenderCopy(renderer,instruction_tex,NULL,NULL);
			SDL_RenderPresent(renderer);
 		}	
 	}

 	if(menu_menu==3){
 		/*Loading the Credit image into memory using SDL Image Library Function*/
		credit = SDL_LoadBMP("Images/credit.bmp");
		/*Loading the Credit Image Data Into the Graphics Hardware's Memory*/
		credit_tex = SDL_CreateTextureFromSurface(renderer,credit);
		SDL_FreeSurface(credit);
		SDL_Event event1;
		bool running = true;
 		while(running){
 			while(SDL_PollEvent(&event1)){
 				if(event1.key.keysym.scancode==SDL_SCANCODE_ESCAPE){	
 					goto here;
 				}
 				switch(event1.type){
 					case SDL_QUIT:
 						running = false;
 						break;
 				}
 			}
 			SDL_RenderCopy(renderer,credit_tex,NULL,NULL);
			SDL_RenderPresent(renderer);
 		}
 	}

 	if(menu_menu==4){
 		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
 	}
	/*Clean Up Resources Before Exiting*/
	SDL_DestroyTexture(life_tex);
	SDL_DestroyTexture(bullet_tex);
	SDL_DestroyTexture(obstacle_tex);
	SDL_DestroyTexture(player_tex);
	SDL_DestroyTexture(background_tex);
	SDL_DestroyTexture(load_tex);
	SDL_DestroyTexture(credit_tex);
	SDL_DestroyTexture(instruction_tex);
	SDL_DestroyTexture(menu_tex);
	SDL_DestroyTexture(pre_menu_tex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	score = currentscore;
	highscore(score);
	return 0;
}

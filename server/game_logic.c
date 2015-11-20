#include <map.h>

#include "game_logic.h"
#include "client.h"

/*TODO FIXME: this whole file would make an italian cheif proud*/
/* seriously, this shits Al dente */

int near(struct vector a, struct vector b)
{
	if(( a.x > (b.x - 1)  && a.x < b.x + 1) &&
			(a.z > (b.z-1) && a.z < (b.z+1))){
		return 1;
	}
	return 0;
}

int player_hittest(struct game_state * gs, struct vector b)
{
	int i;
	for(i=0;i<gs->n_players;i++){
		if(near(gs->game_player[i].location,b))
			return i;
	}
	return -1;
}

void bullet_update(struct game_state * gs, double delta)
{
	int i,j;
	double s = gs->bullet[i].speed*delta;
	for(i=0;i<gs->n_bullets;i++){
		gs->bullet[i].location.x+=
			gs->bullet[i].direction.x * s;
		gs->bullet[i].location.z+=
			gs->bullet[i].direction.z * s;
		if(world_map.tiles[(int)gs->bullet[i].location.x]
				[(int)gs->bullet[i].location.z])
			remove_bullet(gs,i);
		j = player_hittest(gs,gs->bullet[i].location);
		if(j!=-1){
			gs->game_player[j].health-=10;
			remove_bullet(gs,i);
		}
	}
}

/* this one in particular, god greif */
void npc_update(struct game_state * gs,double delta)
{
	int i,j;
	for(j=0;j<gs->n_npcs;j++){
		if(gs->npc[j].type == UNIT_TYPE_SHOP){
			for(i=0;i<gs->n_players;i++){
				if(near(gs->game_player[i].location,
							gs->npc[j].location)){
					if(gs->game_player[i].inventory.n_items){
						gs->game_player[i].inventory.item[1]=flag_item();
						gs->game_player[i].inventory.n_items++;
					}
				}
			}
		}
		if(gs->npc[j].type == UNIT_TYPE_COIN){
			for(i=0;i<gs->n_players;i++){
				if(near(gs->game_player[i].location,gs->npc[j].location)){
					if(strcmp(gs->game_player[i].inventory.item[3].name,"coin")==0){
						gs->game_player[i].inventory.item[3].amount++;
					}else{
						gs->game_player[i].inventory.item[3]=coin_item();
						gs->game_player[i].inventory.n_items++;
					}
				}
			}
		}
	}
}

void fire_bullet(struct vector starting, struct vector direction)
{
	struct bullet b;
	b.location.x = starting.x+direction.x;
	b.location.y = starting.y+direction.y;
	b.location.z = starting.z+direction.z;
	b.direction = direction;
	b.speed = 5;
	b.duration = 100;
	add_bullet(&world_state,b);
}

void move_unit(struct unit * u,struct vector d)
{
	if(!world_map.tiles[ (int)(u->location.x+d.x) ][(int)(u->location.z)])
		u->location.x+=d.x;
	if(!world_map.tiles[ (int)(u->location.x)][(int)(u->location.z+d.z)])
		u->location.z+=d.z;
}

void player_movement(struct game_state * gs, double delta, int i)
{
	double d = gs->game_player[i].speed*delta;
	struct vector dvec = (struct vector){0,0,0};
	if(clients[i].pi.keys['W']){
		dvec.x-=d;
		dvec.z-=d;
	}
	if(clients[i].pi.keys['A']){
		dvec.x-=d;
		dvec.z+=d;
	}
	if(clients[i].pi.keys['S']){
		dvec.x+=d;
		dvec.z+=d;
	}
	if(clients[i].pi.keys['D']){
		dvec.x+=d;
		dvec.z-=d;
	}
	move_unit(&gs->game_player[i],dvec);
}

void player_attack(struct game_state * gs, double delta, int i)
{
	//TODO temporary firing rate definition
#define FR 4
	if(gs->game_player[i].cooldown<1){
		if(clients[i].pi.keys['J']){
			fire_bullet(gs->game_player[i].location,
					(struct vector) {1,0,1});
			gs->game_player[i].cooldown = FR;
		}
		if(clients[i].pi.keys['H']){
			fire_bullet(gs->game_player[i].location,
					(struct vector) {-1,0,1});
			gs->game_player[i].cooldown = FR;
		}
		if(clients[i].pi.keys['K']){
			fire_bullet(gs->game_player[i].location,
					(struct vector) {-1,0,-1});
			gs->game_player[i].cooldown = FR;
		}
		if(clients[i].pi.keys['L']){
			fire_bullet(gs->game_player[i].location,
					(struct vector) {1,0,-1});
			gs->game_player[i].cooldown = FR;
		}
	} else {
		gs->game_player[i].cooldown-=10*delta;
	}
}

void respawn(struct game_state * gs, struct unit * u)
{
	if(u->team==RED_TEAM)
		u->location=gs->red.spawn;
	if(u->team==BLUE_TEAM)
		u->location=gs->blue.spawn;
	u->health=100;
}

void player_update(struct game_state * gs,double delta)
{
	int i = 0;
	for(i=0;i<n_clients;i++){
		player_movement(gs,delta,i);
		player_attack(gs,delta,i);
		if(gs->game_player[i].health < 0)
			respawn(gs,&gs->game_player[i]);
	}
}

void flag_update(struct game_state * gs,double delta)
{
	int i = 0;
	for(i=0;i<n_clients;i++){
		/*TODO replace this all with pickup item */
		if(near(gs->game_player[i].location,gs->blue.flag.location)&&
				(gs->game_player[i].team==RED_TEAM)){
			gs->blue.flag.location=gs->game_player[i].location;
			if(near(gs->red.flag_starting,gs->blue.flag.location)){
				gs->red.score++;
				gs->blue.flag.location=gs->blue.flag_starting;
			}
		}
		/*TODO FIXME; this one is temporarily different for testing*/
		if(near(gs->game_player[i].location,gs->red.flag.location)&&
				(gs->game_player[i].team==BLUE_TEAM)){

			gs->red.flag.location=gs->game_player[i].location;
			gs->game_player[i].inventory.item[0]=flag_item();
			gs->game_player[i].inventory.n_items=1;

			if(near(gs->red.flag.location,gs->blue.flag_starting)){
				gs->blue.score++;
				gs->red.flag.location=gs->red.flag_starting;
				gs->game_player[i].inventory.item[0]=(struct item) {0};
				gs->game_player[i].inventory.n_items=0;
			}
		}
	}
}


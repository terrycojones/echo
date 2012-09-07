#ifndef lint
static char *rcsid = "$Header: /disks/disk1/Users/terry/r/echo/s/RCS/interaction.c,v 1.7 1993/12/08 07:06:20 terry Exp terry $";
#endif

#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "types.h"
#include "globals.h"
#include "agents.h"
#include "sites.h"
#include "resources.h"
#include "interaction.h"
#include "random.h"

#ifndef NO_COMBAT
BOOLEAN
will_attack(a1, a2)
AGENT *a1;
AGENT *a2;
{
    /*
     * Return TRUE if agent a1 will atack agent a2.
     */

	rcsid = rcsid;
    
#ifdef CHECK_ARGS
    if (!a1){
	error("will_attack() called with NULL a1!");
    }
    
    if (!a2){
	error("will_attack() called with NULL a2!");
    }
#endif /* CHECK_ARGS */

    if (AGENT_OFFENSE_TAG_LEN(a2) >= AGENT_COMBAT_COND_LEN(a1) &&
	!memcmp(AGENT_COMBAT_COND(a1), AGENT_OFFENSE_TAG(a2), AGENT_COMBAT_COND_LEN(a1))){
	
	if DEBUG("C") {
	    printf("COMBAT: the following two agents will fight!\n");
	    print_agent(a1, stdout);
	    print_agent(a2, stdout);
	}
	
	return TRUE;
    }

    return FALSE;
}
#endif

#ifndef NO_TRADE
BOOLEAN
will_trade(a1, a2)
AGENT *a1;
AGENT *a2;
{
    /*
     * Return TRUE if agent a1 will trade with agent a2 AND vice-versa.
     */

#ifdef CHECK_ARGS
    if (!a1){
	error("will_trade() called with NULL a1!");
    }
    
    if (!a2){
	error("will_trade() called with NULL a2!");
    }
#endif /* CHECK_ARGS */

    if (!memcmp(AGENT_TRADE_COND(a1), AGENT_OFFENSE_TAG(a2), AGENT_TRADE_COND_LEN(a1)) &&
	!memcmp(AGENT_TRADE_COND(a2), AGENT_OFFENSE_TAG(a1), AGENT_TRADE_COND_LEN(a2))){
	
	if DEBUG("T") {
	    printf("TRADE: the following two agents will trade!\n");
	    print_agent(a1, stdout);
	    print_agent(a2, stdout);
	}
	
	return TRUE;
    }

    return FALSE;
}
#endif

#ifndef NO_SEX
BOOLEAN
will_mate(a1, a2)
AGENT *a1;
AGENT *a2;
{
    /*
     * Return TRUE if agent a1 will mate with agent a2 AND vice-versa.
     *
     * There are two ways in which an agent can find another acceptable:
     *
     * 1) The agent has a zero length mating condition and the other
     *    agent has a zero length mating tag.
     *
     * 2) The agent has a non-zero length mating condition and this is
     *    a prefix (not necessarily a proper one) of the other agent's
     *    mating tag. Thus if agent one has a mating condition of
     *    "abc", it will accept a mating tag of either "abc" or "abcd",
     *    but will not accept "ab".
     */

    /* We really only need one of these variables, but this is clearer. */
    BOOLEAN a1_accepts_a2 = FALSE;
    BOOLEAN a2_accepts_a1 = FALSE;

#ifdef CHECK_ARGS
    if (!a1){
	error("will_mate() called with NULL a1!");
    }
    
    if (!a2){
	error("will_mate() called with NULL a2!");
    }
#endif /* CHECK_ARGS */

    /*
     * Check to see if agent 1 finds agent 2 acceptable as a mate.
     */
    
    if (AGENT_MATING_COND_LEN(a1) == 0){
	if (AGENT_MATING_TAG_LEN(a2) == 0){
	    a1_accepts_a2 = TRUE;
	}
    }
    else {
	if (AGENT_MATING_TAG_LEN(a2) >= AGENT_MATING_COND_LEN(a1)){
	    if (!memcmp(AGENT_MATING_COND(a1), AGENT_MATING_TAG(a2), AGENT_MATING_COND_LEN(a1))){
		a1_accepts_a2 = TRUE;
	    }
	}
    }

    /*
     * If agent 1 doesn't accept agent 2, we can return (FALSE) now
     * since mating (in Echo) must be by mutual agreement.
     */
    
    if (a1_accepts_a2 == FALSE){
	return FALSE;
    }

    /*
     * Agent 1 accepts agent 2, but what about the other way round?
     */
    
    if (AGENT_MATING_COND_LEN(a2) == 0){
	if (AGENT_MATING_TAG_LEN(a1) == 0){
	    a2_accepts_a1 = TRUE;
	}
    }
    else {
	if (AGENT_MATING_TAG_LEN(a1) >= AGENT_MATING_COND_LEN(a2)){
	    if (!memcmp(AGENT_MATING_COND(a2), AGENT_MATING_TAG(a1), AGENT_MATING_COND_LEN(a2))){
		a2_accepts_a1 = TRUE;
	    }
	}
    }

    /* We can simply return this since we know 1 accepts 2. */
    return a2_accepts_a1;
}
#endif

#ifndef NO_COMBAT
#ifdef BUGGY_COMBAT_CALCULATION
RESOURCE_SIZE
combat_payoff(a1, a2, combat_payoff_matrix)
AGENT *a1;
AGENT *a2;
RESOURCE_SIZE **combat_payoff_matrix;
{
    /*
     * Return the combat payoff to agent a1 in a fight against agent a2.
     */
     
    RESOURCE_SIZE payoff = (RESOURCE_SIZE)0;
    CHROMOSOME_LENGTH len;
    register int i;
    
#ifdef CHECK_ARGS
    if (!a1){
	error("combat_payoff() called with NULL a1!");
    }
    
    if (!a2){
	error("combat_payoff() called with NULL a2!");
    }
#endif /* CHECK_ARGS */

    len = AGENT_OFFENSE_TAG_LEN(a1);
    
    /* Make sure len has the lesser of the two lengths. */
    if (len > AGENT_DEFENSE_TAG_LEN(a2)){
	len = AGENT_DEFENSE_TAG_LEN(a2);
    }

    for (i = 0; i < len; i++){
	payoff += combat_payoff_matrix[AGENT_OFFENSE_TAG(a1)[i] - 'a'][AGENT_DEFENSE_TAG(a2)[i] - 'a'];
    } 
    
    return payoff;
}

#else

double
combat_payoff(a1, a2, combat_payoff_matrix)
AGENT *a1;
AGENT *a2;
RESOURCE_SIZE **combat_payoff_matrix;
{
    /*
     * Return the combat payoff to agent a1 in a fight against agent a2.
     */
     
    double ratio;
    double total_match;
    double len1;
    double len2;
    CHROMOSOME_LENGTH min_len;
    register int i;
    
#ifdef CHECK_ARGS
    if (!a1){
	error("combat_payoff() called with NULL a1!");
    }
    
    if (!a2){
	error("combat_payoff() called with NULL a2!");
    }
#endif /* CHECK_ARGS */

    len1 = (double) AGENT_OFFENSE_TAG_LEN(a1);
    len2 = (double) AGENT_DEFENSE_TAG_LEN(a2);

    if (len1 == 0.0){
	return 1.0;
    }

    if (len2 == 0.0){
	return len1 / SMALLEST_LENGTH_RATIO_DENOM;
    }
    
    min_len = len1 < len2 ? (CHROMOSOME_LENGTH) len1 : (CHROMOSOME_LENGTH) len2;
    
    ratio = len1 / len2;

    for (total_match = 0.0, i = 0; i < min_len; i++){
	total_match += (double) combat_payoff_matrix[AGENT_OFFENSE_TAG(a1)[i] - 'a'][AGENT_DEFENSE_TAG(a2)[i] - 'a'];
    }

    return total_match / (double) min_len * ratio;
}
#endif /* BUGGY_COMBAT_CALCULATION */
#endif /* NO_COMBAT */

void
interact(a1, a2, site, trade_fraction)
AGENT_POPULATION_SIZE a1;
AGENT_POPULATION_SIZE a2;
SITE *site;
FRACTION trade_fraction;
{
    /*
     * Carry out an interaction between the agents a1 and a2 at site.
     * a1 is the agressor and has been moved next to a2.
     */
     
    /* These pointers will save on a bunch of derefencing and make the code easier to read. */
    AGENT *agent1;
    AGENT *agent2;
    int a1_attacks_a2 = 1;
    AGENT_POPULATION_SIZE instance1 = -1;
    AGENT_POPULATION_SIZE instance2 = -1;
    

#ifdef CHECK_ARGS
    if (!site){
	error("interact() called with NULL site!");
    }
    
    if (SITE_AGENT(site, a1) == (AGENT *)0){
	error("interact() called with empty a1 agent!");
    }
    
    if (SITE_AGENT(site, a2) == (AGENT *)0){
	error("interact() called with empty a2 agent!");
    }
#endif /* CHECK_ARGS */
    
    
    /* Is there only one agent here? If so, there's no point in doing combat, trade etc. */
    if (a1 == a2){
	return;
    }

    agent1 = SITE_AGENT(site, a1);
    agent2 = SITE_AGENT(site, a2);
    
    if DEBUG("IikcCtmDM"){
	instance1 = agent_instance(agent1);
	instance2 = agent_instance(agent2);
    }

    if DEBUG("I") printf("%s%03d will interact with %s%03d (locations %d and %d)\n", 
			 AGENT_NAME(agent1), instance1, AGENT_NAME(agent2), instance2, a1, a2);
    if DEBUG("i") printf("%s%03d will interact with %s%03d\n", 
			 AGENT_NAME(agent1), instance1, AGENT_NAME(agent2), instance2);

#ifndef NO_COMBAT
    if (will_attack(agent1, agent2) || (a1_attacks_a2 = 0) || will_attack(agent2, agent1)){
    
	/*
	 * a1_attacks_a2 was initialized to 1. It will only be 0 now if agent2 attacks
	 * agent1. This saves on having to either duplicate a bunch of code or call
	 * will_attack again for one of the pairs. Ugly, but faster (and documented!).
	 */
	 
	AGENT *winner;
	AGENT *loser;
	PROBABILITY p_a1_wins;
	
#ifdef BUGGY_COMBAT_CALCULATION
	RESOURCE_SIZE payoff_to_a1 = combat_payoff(agent1, agent2, WORLD_COMBAT(the_world));
	RESOURCE_SIZE payoff_to_a2 = combat_payoff(agent2, agent1, WORLD_COMBAT(the_world));

	/*
	 * I now think this is a bit strange! If they get the same score, surely
	 * they can still fight it out.
	 */
	
	/* No-one wins. */
	if (payoff_to_a1 == payoff_to_a2){
	    return;
	}
	
	/* This can't be / by 0, since for that both would need to be 0 and thus equal. */
	p_a1_wins = (double) payoff_to_a1 / (double) (payoff_to_a2 + payoff_to_a1);

	if DEBUG("c") printf("%s%03d (agent1) payoff is %d, %s%03d (agent2) payoff is %d\n", AGENT_NAME(agent1), instance1,
			     payoff_to_a1, AGENT_NAME(agent2), instance2, payoff_to_a2);
#else
	double payoff_to_a1 = combat_payoff(agent1, agent2, WORLD_COMBAT(the_world));
	double payoff_to_a2 = combat_payoff(agent2, agent1, WORLD_COMBAT(the_world));

	if (payoff_to_a1 == payoff_to_a2){
	    p_a1_wins = 0.5;
	}
	else {
	    p_a1_wins = payoff_to_a1 / (payoff_to_a2 + payoff_to_a1);
	}

	if DEBUG("c") printf("%s%03d payoff = %f (P(win)=%f), %s%03d payoff = %f\n", AGENT_NAME(agent1), instance1,
			     payoff_to_a1, p_a1_wins, AGENT_NAME(agent2), instance2, payoff_to_a2);
#endif
	
	if (a1_attacks_a2){

	    AGENT_POPULATION_SIZE nagents = SITE_NAGENTS(site);
	    
	    /* 
	     * Agent1 is attacking agent2. Give the latter a chance to flee. 
	     *
	     * Neither of these divisions can be by zero, the condition and
	     * the above equality test ensure this. e.g if the payoffs are not the
	     * same and a1 > a2, then a1 != 0.
	     *
	     * The if first checks to see that there is actually somewhere to go.
	     */

	    if ((nagents > 3 || (nagents == 3 && a1 != 2)) && biased_coin_flip(p_a1_wins)){
		
		/* Agent2 leaves. */
		
		if (nagents == 3){
		    /* 
		     * We know that a1 is not the middle agent, so a2 must be since
		     * it is next to a1. Do a simple swap.
		     */
		     
		    AGENT *tmp;

		    if (a1 == 0){
			tmp = SITE_AGENT(site, 2);
			SITE_AGENT(site, 2) = SITE_AGENT(site, 1);
		    }
		    else {
			tmp = SITE_AGENT(site, 0);
			SITE_AGENT(site, 0) = SITE_AGENT(site, 1);
		    }
		    
		    SITE_AGENT(site, 1) = tmp;
		}
		else {
		    do {
			PROBABILITY d = knuth_random();
			AGENT_POPULATION_SIZE offset = d * d * d * nagents + 2;
			
			if (a2 + offset < nagents){
			    if (a2 - offset >= 0){
				/* Can go either way. */
				if (random_boolean() == TRUE){
				    if DEBUG("cC") printf("%s%03d flees %s%03d from location %d to %d\n", AGENT_NAME(agent2), instance2,
						      AGENT_NAME(agent1), instance1, a2, a2 + offset);
				    site_move_agents_together(a2, a2 + offset, site);
				    break;
				}
				else {
				    if DEBUG("cC") printf("%s%03d flees %s%03d from location %d to %d\n", AGENT_NAME(agent2), instance2,
						      AGENT_NAME(agent1), instance1, a2, a2 - offset);
				    site_move_agents_together(a2, a2 - offset, site);
				    break;
				}
			    }
			    else {
				/* Can only go up. */
				if DEBUG("cC") printf("%s%03d flees %s%03d from location %d to %d\n", AGENT_NAME(agent2), instance2,
						      AGENT_NAME(agent1), instance1, a2, a2 + offset);
				site_move_agents_together(a2, a2 + offset, site);
				break;
			    }
			}
			else {
			    if (a2 - offset >= 0){
				/* Can only go down. */
				if DEBUG("cC") printf("%s%03d flees %s%03d from location %d to %d\n", AGENT_NAME(agent2), instance2,
						      AGENT_NAME(agent1), instance1, a2, a2 - offset);
				site_move_agents_together(a2, a2 - offset, site);
				break;
			    }
			}
		    } while(1);
		}
		
		
		return;
	    }
	}

	/*
	 * The combat will happen. Compute the winner probabilistically.
	 */

	if (biased_coin_flip(p_a1_wins)){
	    winner = agent1;
	    loser = agent2;
	}
	else {
	    winner = agent2;
	    loser = agent1;
	}

	/*
	 * We have a winner. Give the loser's resources (chromosomes and reservoir) to the winner
	 * and delete the loser.
	 */
	add_resources(AGENT_RESERVE_LIST(winner), AGENT_RESERVE_LIST(loser));
	add_resources(AGENT_RESERVE_LIST(winner), AGENT_MAKEUP_LIST(loser));
	AGENT_GAINED(winner) = TRUE;

	if (winner == agent1){
	    if DEBUG("CckD") printf("%s%03d kills %s%03d\n", AGENT_NAME(agent1), instance1, AGENT_NAME(agent2), instance2);
	    site_delete_agent(site, a2, TRUE, FALSE);
	}
	else {
	    if DEBUG("CckD") printf("%s%03d kills %s%03d\n", AGENT_NAME(agent2), instance2, AGENT_NAME(agent1), instance1);
	    site_delete_agent(site, a1, TRUE, FALSE);
	}
    }
    else
#endif /* NO_COMBAT */
    {
	/* NO COMBAT. Check for more friendly interactions. */
#ifndef NO_TRADE
	if (will_trade(agent1, agent2)){
	    
	    NRESOURCES a1_resource_index = AGENT_TRADING_RESOURCE(agent1) - 'a';
	    NRESOURCES a2_resource_index = AGENT_TRADING_RESOURCE(agent2) - 'a';

	    RESOURCE_SIZE a1_trade_amount =
		(RESOURCE_SIZE)(trade_fraction * (double) AGENT_EXCESS(agent1, a1_resource_index));
	    RESOURCE_SIZE a2_trade_amount =
		(RESOURCE_SIZE)(trade_fraction * (double) AGENT_EXCESS(agent2, a2_resource_index));

	    if DEBUG("t") printf("%s%03d trades with %s%03d\n", AGENT_NAME(agent1), instance1, AGENT_NAME(agent2), instance2);
	    
	    /* Redistribute the trading resources. */
	    AGENT_RESERVE(agent1, a1_resource_index) -= a1_trade_amount;
	    AGENT_RESERVE(agent2, a1_resource_index) += a1_trade_amount;
	    AGENT_RESERVE(agent1, a2_resource_index) += a2_trade_amount;
	    AGENT_RESERVE(agent2, a2_resource_index) -= a2_trade_amount;
	    
	    if (a2_trade_amount > (RESOURCE_SIZE) 0){
		AGENT_GAINED(agent1) = TRUE;
	    }
	    
	    if (a1_trade_amount > (RESOURCE_SIZE) 0){
		AGENT_GAINED(agent2) = TRUE;
	    }
	}
#endif
	
#ifndef NO_SEX
	if (sex == TRUE && biased_coin_flip(SITE_P_CROSSOVER(site)) &&
	    will_mate(agent1, agent2) == TRUE)
	{
	    if DEBUG("XM") printf("%s%03d mates with %s%03d\n", AGENT_NAME(agent1), instance1, AGENT_NAME(agent2), instance2);
	    agent_crossover(agent1, agent2, site);
	}
#endif

    }
    
    return;
}

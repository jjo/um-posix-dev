void *jj_critic_new(void);	/* alloca un nuevo lock */
int jj_critic_on(void *);	/* prende el lock en forma exclusiva*/
int jj_critic_off(void *);	/* apaga el lock */

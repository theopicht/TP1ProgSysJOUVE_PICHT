# TP1ProgSysJOUVE_PICHT

#Question 1 

On créer un fichier enseash, permettant d'afficher le message d'acceuil à l'aide de l'instruction suivante "write(STDOUT_FILENO, welcome_message, sizeof(welcome_message) - EXCLUDE_NULL_CHAR)"  
Cela permet de ne pas utiliser l'instruction printf()  

Résultats Shell : 

![Image 29-11-2023 à 14 23](https://github.com/theopicht/TP1ProgSysJOUVE_PICHT/assets/151057454/d149a436-349a-4f75-9dd9-7bcb7e833c4b)  

#Question 2

On implémente alors la lecture de la commande saisie à l'aide de la fonction "strcmp()". L'appel à la l'instruction "execlp("fortune", "fortune", NULL)" permet d'afficher en retour des citations. Dans le cas où aucune saisie n'est faite, on teste la longueur de la commande saisie ("command_length == 0") et on renvoie la date actuelle.  

Résultats Shell : 

<img width="627" alt="Capture d’écran 2023-11-29 à 14 53 57" src="https://github.com/theopicht/TP1ProgSysJOUVE_PICHT/assets/151057454/0abc77cb-3bb3-427e-81fc-c6412740bb30">

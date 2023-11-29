# TP1ProgSysJOUVE_PICHT

---

## Question 1 

On créer un fichier enseash, permettant d'afficher le message d'acceuil à l'aide de l'instruction suivante "write(STDOUT_FILENO, welcome_message, sizeof(welcome_message) - EXCLUDE_NULL_CHAR)"  
Cela permet de ne pas utiliser l'instruction printf()  

<ins>Résultats Shell</ins> :  

![Image 29-11-2023 à 14 23](https://github.com/theopicht/TP1ProgSysJOUVE_PICHT/assets/151057454/d149a436-349a-4f75-9dd9-7bcb7e833c4b)  

---

## Question 2

On implémente alors la lecture de la commande saisie à l'aide de la fonction "strcmp()". L'appel à la l'instruction "execlp("fortune", "fortune", NULL)" permet d'afficher en retour des citations. Dans le cas où aucune saisie n'est faite, on teste la longueur de la commande saisie ("command_length == 0") et on renvoie la date actuelle.  

<ins>Résultats Shell</ins> :  

<img width="627" alt="Capture d’écran 2023-11-29 à 14 53 57" src="https://github.com/theopicht/TP1ProgSysJOUVE_PICHT/assets/151057454/0abc77cb-3bb3-427e-81fc-c6412740bb30">  

<img width="603" alt="Capture d’écran 2023-11-29 à 15 17 58" src="https://github.com/theopicht/TP1ProgSysJOUVE_PICHT/assets/151057454/e2331611-c48f-4f6d-9857-67ed31779b7d">  

---

## Question 3  

Ici, deux cas se présente à nous pour sortie du shell :  
- le message saisie est "exit" --> on utilise l'instruction "strcmp", puis affichage du message "Bye bye..." à l'aide d'un "write", puis sortie du shell.  
- la commande faite est <ctrl>+d --> on utilise l'instruction "command_length == 0", du message "Bye bye..." à l'aide d'un "write", puis sortie du shell (avec remplacement du retour à la ligne par un caractère NULL (\0))  

<ins>Cas "exit"</ins> :  
<img width="603" alt="Capture d’écran 2023-11-29 à 16 14 42" src="https://github.com/theopicht/TP1ProgSysJOUVE_PICHT/assets/151057454/57371a03-91b0-44e8-be78-2788d493fce8">

<ins>Cas "ctrl+d"</ins> :  
<img width="603" alt="Capture d’écran 2023-11-29 à 16 16 10" src="https://github.com/theopicht/TP1ProgSysJOUVE_PICHT/assets/151057454/d99f57d2-b8eb-4460-b986-62a365f7e2e3">  

---

## Question 4


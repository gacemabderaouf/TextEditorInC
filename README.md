# TextEditorInC
                Fondement du projet:

  Soit un texte donné en langue française, on désire implémenter quelques fonctions de traitement de
texte, telle que chercher un mot, chercher et remplacer un mot par un autre avec des options, ....etc


                     Travail à faire
					 
  Il s’agit d’écrire un programme en C de manipulation de fichier texte. Tous les traitements se font
à la demande par le biais d’un menu principal
— Afficher le fichier page par page avec les informations suivantes : Nombre de caractères,
nombre de mots, nombre de lignes et nombre de pages.
— Implémenter les fonctions de formatage des paragraphes du fichier avec les fonctions aligner à
gauche, à droite, à gauche et à droite, et centrer. En insérant des symboles devant et après le
paragraphe qu’on veut traiter(préalabelement). Par exemple si on met /c/ devant et après un paragraphe,
il sera centré.
— Implémenter la fonction chercher et remplacer avec ses différentes formules.Quand le mot recherché n’est pas trouvé, affichez le ou les mots qui se rapprochent le plus. . Si c’est une recherche approximative elle doit être signalée.
Pour réaliser cette fonction, il faudra construire un fichier index contenant tous les mots du fichier
avec leur emplacement. La recherche d’un mot se fera dans l’index. En cas de remplacement mettre
à jour l’index.

                  Principe de fonctionnement
				  
1-Lorque l'utilisateur ouvre le fichier, on le parcour une promiere fois en faisant une 
analyse syntaxique (calcule du nombre de balise...);
2-On fait un autre parcour qui depend du premier pour indexer les coords des pages, calculer le nombre de mots,le nombre de lignes...
et indexer aussi les mot dans plusieures fichiers index triés (Pour faire une recherche dichotomique) en indexant les lignes aussi.
3-Pour remplacer un mot on crée un nouveau fichier et aller à 1.

                utilisation du programme réalisé
				
1-Entrez le nom d'un fichier existant et correctement sinon le programme terminera
2-Utilisez le flèches "haut et bas" pour allez à la page precedente/suivante.
3-Cliquer un nombe [0-9] un message s'affichera, faites entrer le numero de la page pour y aller.
4-Cliquer sur R/r pour rechercher un mot s'il existe il sera coloré et pour aller a l'ocurrence suivante cliquer sur Entrée, si vous voulez annuler la recherche cliquer Echap, s'il n'existe pas 
un message s'affichera et vous pouver proceder une recherche approximative.
5-Pour remplacer un mot il faut le chercher tout d'abord puis cliquer sur F/f, un message s'affichera Et faites entrer le nouveau mot, pour remplacer l'occurence suivante cliquer Entrée puis une autre F/f.

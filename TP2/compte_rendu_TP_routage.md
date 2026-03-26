# Compte-rendu — TP Routage RSX2
**Nom / Prénom :**  
**Date :**  
**Binôme :**  

---

## Plan d'adressage IPv4 (à remplir dès le début, garder sous la main)

| Routeur | Interface | Réseau | Adresse IP/CIDR |
|---------|-----------|--------|-----------------|
| R1 | eth0 | a | |
| R1 | eth1 | b | |
| R2 | eth0 | a | |
| R2 | eth1 | A | |
| R3 | eth0 | b | |
| R3 | eth1 | B | |

---

## Section 1 — Routage statique

---

### Question 1 — Table de routage initiale des routeurs

**Commandes utilisées :**
```bash
ip route
```

**Capture R1 :**
<!-- Insérer capture ici -->

**Capture R2 :**
<!-- Insérer capture ici -->

**Capture R3 :**
<!-- Insérer capture ici -->

**Constat :**

> *(Que constatez-vous ? Les tables sont-elles vides ? Pourquoi ?)*

---

### Question 2 — Configuration et activation des interfaces

**Commandes utilisées sur R1 :**
```bash
# À compléter
```

**Commandes utilisées sur R2 :**
```bash
# À compléter
```

**Commandes utilisées sur R3 :**
```bash
# À compléter
```

**Vérification — `ip address` sur R1 :**
<!-- Insérer capture ici -->

**Vérification — `ip address` sur R2 :**
<!-- Insérer capture ici -->

**Vérification — `ip address` sur R3 :**
<!-- Insérer capture ici -->

**Vérification ping :**
<!-- Insérer capture ici -->

**Explication — comment vérifier que la configuration est correcte :**

> *(Expliquez ce que vous avez vérifié et comment)*

---

### Question 3 — Table de routage après configuration des interfaces

**Commandes utilisées :**
```bash
ip route
```

**Table de routage R1 :**
<!-- Insérer capture ici -->

**Table de routage R2 :**
<!-- Insérer capture ici -->

**Table de routage R3 :**
<!-- Insérer capture ici -->

**Pourquoi les tables ne sont-elles pas vides ?**

>

**Particularité des routes déjà présentes :**

>

**Signification de « scope link » :**

>

---

### Question 4 — Configuration des routes statiques

**Commandes ajoutées sur R1 :**
```bash
# À compléter
```

**Commandes ajoutées sur R2 :**
```bash
# À compléter
```

**Commandes ajoutées sur R3 :**
```bash
# À compléter
```

**Table de routage finale R1 :**
<!-- Insérer capture ici -->

**Table de routage finale R2 :**
<!-- Insérer capture ici -->

**Table de routage finale R3 :**
<!-- Insérer capture ici -->

**Vérification pings (vers les 4 réseaux depuis chaque routeur) :**
<!-- Insérer capture ici -->

---

## Section 2 — Traceroute

> ⚠️ Captures Wireshark actives sur eth0 et eth1 de R1 avant de commencer.

---

### Question 1 — Traceroute depuis R2 vers réseau B

**Commande utilisée :**
```bash
traceroute -q1 -N1 <IP_eth1_R3>
```

**Sortie de la commande :**
<!-- Insérer capture ici -->

---

### Question 2 — Que renvoie traceroute ?

>

---

### Question 3 — Correspondance des adresses renvoyées

| Adresse IP renvoyée | Routeur | Interface |
|---------------------|---------|-----------|
| | | |
| | | |

---

### Question 4 — TTL du 1er paquet envoyé par R2

**Capture Wireshark (1er paquet) :**
<!-- Insérer capture ici -->

**Valeur du TTL :**

>

---

### Question 5 — Que transporte ce paquet ?

**Capture Wireshark :**
<!-- Insérer capture ici -->

**Réponse :**

>

---

### Question 6 — Ce paquet parvient-il à destination ?

**Réponse :**

>

**Pourquoi :**

>

---

### Question 7 — Qui répond ? Quel message ?

**Capture Wireshark de la réponse :**
<!-- Insérer capture ici -->

**Qui répond :**

>

**Protocole / Type / Code :**

>

**Signification :**

>

---

### Question 8 — TTL du 2ème paquet

**Capture Wireshark :**
<!-- Insérer capture ici -->

**Valeur du TTL :**

>

---

### Question 9 — Ce paquet parvient-il à destination ?

**Réponse :**

>

**Pourquoi :**

>

---

### Question 10 — Qui répond ? Quel message ?

**Capture Wireshark :**
<!-- Insérer capture ici -->

**Qui répond :**

>

**Protocole / Type / Code :**

>

**Signification :**

>

---

### Question 11 — Messages permettant à traceroute de connaître les IPs des routeurs

>

---

### Question 12 — Pourquoi l'IP de eth0 de R3 n'est-elle pas renvoyée ?

>

---

## Section 3 — Tracepath

---

### Question 1 — Configurer MTU à 1000 sur la liaison b

**Commandes utilisées :**
```bash
# Sur R1 :

# Sur R3 :
```

**Vérification :**
<!-- Insérer capture ici -->

---

### Question 2 — Ping 1200 octets avec `-M dont` depuis R2

**Commande utilisée :**
```bash
ping -c1 -s 1200 -M dont <IP_réseau_B>
```

**Sortie de ping :**
<!-- Insérer capture ici -->

**Ce qu'affiche la commande :**

>

---

### Question 3 — La requête parvient-elle ? Différence avant/après R1 ?

**Capture Wireshark eth0 de R1 (avant transmission) :**
<!-- Insérer capture ici -->

**Capture Wireshark eth1 de R1 (après transmission) :**
<!-- Insérer capture ici -->

**La requête parvient-elle à destination ?**

>

**Différence entre la requête envoyée par R2 et après transmission par R1 :**

>

---

### Question 4 — Différence sur la réponse ? R1 réassemble-t-il ?

**Capture Wireshark :**
<!-- Insérer capture ici -->

**Y a-t-il une différence sur la réponse ?**

>

**R1 réassemble-t-il le message ICMP ?**

>

---

### Question 5 — Ping 1200 octets SANS `-M dont`

**Commande utilisée :**
```bash
ping -c1 -s 1200 <IP_réseau_B>
```

**Sortie de ping :**
<!-- Insérer capture ici -->

**Ce qu'affiche la commande :**

>

---

### Question 6 — La requête parvient-elle ? Valeur du flag DF ?

**Capture Wireshark (flag DF dans l'entête IP) :**
<!-- Insérer capture ici -->

**La requête parvient-elle ?**

>

**Raison :**

>

**Valeur du flag « don't fragment » :**

>

---

### Question 7 — Quel avertissement R1 renvoie-t-il à R2 ?

**Capture Wireshark du message ICMP :**
<!-- Insérer capture ici -->

**Type / Code / Signification :**

>

---

### Question 8 — Tracepath depuis R2 vers réseau B

**Commande utilisée :**
```bash
tracepath <IP_réseau_B>
```

**Sortie de tracepath :**
<!-- Insérer capture ici -->

**Ce qu'affiche la commande :**

>

---

### Question 9 — Que transportent les paquets envoyés par R2 ?

**Capture Wireshark :**
<!-- Insérer capture ici -->

**Réponse :**

>

---

### Question 10 — Valeur du flag « don't fragment »

**Capture Wireshark :**
<!-- Insérer capture ici -->

**Valeur du flag DF :**

>

---

### Question 11 — Différences dans l'entête IP des paquets successifs

**Capture Wireshark :**
<!-- Insérer capture ici -->

| N° paquet | TTL | Taille |
|-----------|-----|--------|
| 1 | | |
| 2 | | |
| 3 | | |
| 4 | | |

**Différences constatées :**

>

---

### Question 12 — Qui répond aux 2 premiers paquets ?

**Capture Wireshark :**
<!-- Insérer capture ici -->

**Qui répond :**

>

**Protocole / Type / Code :**

>

**Signification :**

>

---

### Question 13 — Les 2 premiers paquets arrivent-ils à destination ?

**Réponse :**

>

**Pourquoi :**

>

---

### Question 14 — Qui répond au 3ème paquet ?

**Capture Wireshark :**
<!-- Insérer capture ici -->

**Qui répond :**

>

**Protocole / Type / Code :**

>

**Signification :**

>

---

### Question 15 — Le 3ème paquet arrive-t-il à destination ?

**Réponse :**

>

**Pourquoi :**

>

---

### Question 16 — Qui répond au 4ème paquet ?

**Capture Wireshark :**
<!-- Insérer capture ici -->

**Qui répond :**

>

**Protocole / Type / Code :**

>

**Signification :**

>

---

### Question 17 — Le 4ème paquet arrive-t-il à destination ?

**Réponse :**

>

**Pourquoi :**

>

---

### Question 18 — Comment tracepath a su que le MTU de b est 1000 ?

**Capture Wireshark (champ Next-Hop MTU dans le message ICMP) :**
<!-- Insérer capture ici -->

**Explication :**

>

---

### Question 19 — Résumé du fonctionnement de tracepath

>

---

### Question 20 — Similitudes entre traceroute et tracepath

>

---

### Question 21 — Différences entre traceroute et tracepath

>

---

## Section 4 — RIP

> ⚠️ Routes statiques supprimées, Quagga/Zebra lancé sur les 3 routeurs, filtre Wireshark `rip` activé.

---

### Question 1 — Activer RIP sur réseau a depuis R1

**Commandes utilisées :**
```
configure terminal
router rip
network <réseau_a>/24
exit
```

**Capture Wireshark après activation :**
<!-- Insérer capture ici -->

**Constat :**

>

---

### Question 2 — Couche OSI, protocole transport, ports, adresse destination

**Capture Wireshark (détail du paquet RIP) :**
<!-- Insérer capture ici -->

**Couche OSI :**

>

**Protocole de couche transport :**

>

**Ports source / destination :**

>

**Adresse IP de destination :**

>

**Ce que représente cette adresse :**

>

---

### Question 3 — Command RIP et version

**Capture Wireshark :**
<!-- Insérer capture ici -->

**Command :**

>

**Version :**

>

---

### Question 4 — Activer RIP sur réseau b, fréquence des messages

**Commandes utilisées :**
```
configure terminal
router rip
network <réseau_b>/24
exit
```

**Capture Wireshark :**
<!-- Insérer capture ici -->

**Constat :**

>

**Fréquence d'envoi des messages :**

>

---

### Question 5 — Contenu des réponses RIP sur a et sur b

**Capture Wireshark réponses sur a :**
<!-- Insérer capture ici -->

**Capture Wireshark réponses sur b :**
<!-- Insérer capture ici -->

**Contenu sur a :**

>

**Contenu sur b :**

>

---

### Question 6 — Activer RIP sur R2 et R3 — intérêt du message de requête

**Commandes sur R2 :**
```
configure terminal
router rip
network <réseau_a>/24
network <réseau_A>/24
exit
```

**Commandes sur R3 :**
```
configure terminal
router rip
network <réseau_b>/24
network <réseau_B>/24
exit
```

**Capture Wireshark (message de requête) :**
<!-- Insérer capture ici -->

**Intérêt du message de requête :**

>

---

### Question 7 — Différences entre réponses RIP de R1 et sa table de routage

**Table de routage R1 (`show ip route`) :**
<!-- Insérer capture ici -->

**Capture Wireshark réponses RIP de R1 :**
<!-- Insérer capture ici -->

**Différences constatées :**

>

**Pourquoi ces différences (explication du mécanisme) :**

>

---

### Question 8 — Métriques

**Capture Wireshark annonces de R2 :**
<!-- Insérer capture ici -->

**Capture Wireshark annonces de R1 :**
<!-- Insérer capture ici -->

**Métrique de A dans les annonces de R2 :**

>

**Métrique de A dans les annonces de R1 :**

>

**Métrique de A dans les annonces de R3 (selon vous) :**

>

**À quoi correspond la métrique :**

>

---

### Question 9 — Tables de routage et métriques

**Table de routage R1 :**
<!-- Insérer capture ici -->

**Table de routage R2 :**
<!-- Insérer capture ici -->

**Table de routage R3 :**
<!-- Insérer capture ici -->

**Si on ajoutait un lien direct R2-R3, quelle serait la métrique de A sur R3 ?**

>

---

### Question 10 — Simulation panne eth1 de R2

**Commande utilisée :**
```bash
ip link set dev eth1 down
```

**Capture Wireshark (messages RIP après panne) :**
<!-- Insérer capture ici -->

**Constat sur les messages RIP reçus et envoyés par R1 :**

>

**Métrique associée au réseau A inaccessible :**

>

**Pourquoi cette valeur est-elle utilisée ? Que signifie-t-elle ?**

>

**Délai entre la panne et la suppression de la route vers A dans R3 :**

>

---

### Question 11 — Désactiver eth0 de R2 — timeout de R1

**Commandes utilisées :**
```bash
ip link set dev eth1 up   # remettre eth1
ip link set dev eth0 down  # désactiver eth0
```

**Capture Wireshark (surveillance des annonces de R1) :**
<!-- Insérer capture ici -->

**Au bout de combien de temps R1 considère A inaccessible (timeout) :**

>

---

### Question 12 — Routeur PIRATE et attaque RIP

**Commandes de lancement et configuration de PIRATE :**
```bash
# À compléter
```

**Commande création dummy0 :**
```bash
ifconfig dummy0 <IP_dans_B> netmask 255.255.255.0 up
```

**Table de routage R2 avant PIRATE :**
<!-- Insérer capture ici -->

**Table de routage R2 après activation RIP sur PIRATE :**
<!-- Insérer capture ici -->

**Constat concernant le réseau B :**

>

**Comment remédier à ce problème :**

>

---

## Section 5 — NAT (bonus)

> ⚠️ Routeurs précédents arrêtés. Nouveaux routeurs lancés : box, PC, R-FAI.

---

### Question 1 — Plan d'adressage privé réseau interne

| Machine | Interface | Réseau | Adresse IP/CIDR |
|---------|-----------|--------|-----------------|
| PC | eth0 | L1 (interne) | |
| box | eth1 | L1 (interne) | |

**Commandes de configuration :**
```bash
# À compléter
```

**Vérification :**
<!-- Insérer capture ici -->

---

### Question 2 — Réseau public réseau externe

| Machine | Interface | Réseau | Adresse IP/CIDR |
|---------|-----------|--------|-----------------|
| box | eth0 | L2 (externe) | |
| R-FAI | eth0 | L2 (externe) | |

**Commandes de configuration :**
```bash
# À compléter
```

**Vérification :**
<!-- Insérer capture ici -->

---

### Question 3 — Configuration NAT sur box

**Commandes utilisées :**
```bash
echo 1 > /proc/sys/net/ipv4/ip_forward
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
```

**Explication de la règle :**

>

---

### Question 4 — Routes nécessaires

**Routes configurées sur PC :**
```bash
# À compléter
```

**Routes configurées sur box :**
```bash
# À compléter
```

**Routes configurées sur R-FAI :**
```bash
# À compléter
```

**Explication :**

>

---

### Question 5 — Captures et analyse NAT

**Capture Wireshark eth1 de box (côté PC) :**
<!-- Insérer capture ici -->

**Capture Wireshark eth0 de box (côté R-FAI) :**
<!-- Insérer capture ici -->

**Adresse source du paquet reçu de PC :**

>

**Adresse source du paquet envoyé vers R-FAI :**

>

**Adresse de destination de la réponse de R-FAI :**

>

**Adresse de la réponse envoyée par box vers PC :**

>

**Conclusion sur l'intérêt du NAT :**

>

---

### Question 6 — Redirection port HTTP vers PC

**Commande netcat sur PC :**
```bash
nc -lp 8080
```

**Règle iptables DNAT sur box :**
```bash
iptables -t nat -A PREROUTING -i eth0 -p tcp --dport 80 -j DNAT --to-destination <IP_PC>:8080
```

**Explication :**

>

---

### Question 7 — Vérification depuis R-FAI

**Commandes utilisées :**
```bash
nmap <IP_externe_box> -p 80
telnet <IP_externe_box> 80
```

**Capture / sortie nmap :**
<!-- Insérer capture ici -->

**Capture / sortie telnet :**
<!-- Insérer capture ici -->

**Résultat et conclusion :**

>

---

*Fin du compte-rendu*

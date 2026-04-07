# Plan TP Routage — RSX2
> Suivi scrupuleux du sujet | ULILLE-FST-FIL-L3S6-RSX2

---

## ⚠️ Avertissement important

Les VMs Netkit **ne sauvegardent aucune configuration** entre les sessions.  
À chaque reprise, il faut tout refaire depuis la mise en place initiale.  
**Garde toujours ton plan d'adressage (TD) sous la main.**

---

## Plan d'adressage (à remplir dès le début)

| Routeur | Interface | Réseau | Adresse IP/CIDR |
|---------|-----------|--------|-----------------|
| R1 | eth0 | a | |
| R1 | eth1 | b | |
| R2 | eth0 | a | |
| R2 | eth1 | A | |
| R3 | eth0 | b | |
| R3 | eth1 | B | |

---

## Topologie

```
        R1
       /  \
    eth0  eth1
      |     |
      a     b
      |     |
    eth0  eth0
      R2    R3
    eth1  eth1
      |     |
      A     B
```

---

## 🔧 Mise en place initiale

> À refaire à chaque session.

```bash
vstart -D R1 --eth0=a --eth1=b
vstart -D R2 --eth0=a --eth1=A
vstart -D R3 --eth0=b --eth1=B
```

**Résultat attendu :** 3 fenêtres de console s'ouvrent (une par routeur).

---

## Section 1 — Routage statique

### Q1 — Afficher la table de routage initiale

```bash
# Sur R1, R2, R3 :
ip route
```
**Résultat attendu :** Tables vides (aucune interface active, aucune route configurée).

---

### Q2 — Configurer et activer les interfaces

```bash
# Sur chaque routeur (adapter les IPs selon le plan d'adressage) :
ip address add <IP/CIDR> dev eth0
ip address add <IP/CIDR> dev eth1
ip link set dev eth0 up
ip link set dev eth1 up

# Vérification :
ip address
ping <IP_voisin_direct>
```
**Résultat attendu :** `ip address` affiche les IPs, ping vers voisin direct répond.

---

### Q3 — Afficher la table de routage après configuration

```bash
# Sur R1, R2, R3 :
ip route
```
**Résultat attendu :** Routes directement connectées apparaissent automatiquement.  
- Les routes présentes ont la mention **`scope link`**  
- `scope link` = destination accessible directement sur le lien local, sans passerelle

---

### Q4 — Configurer les routes statiques

```bash
# Sur R1 (ajouter A et B) :
ip route add <réseau_A>/24 via <IP_R2_eth0>
ip route add <réseau_B>/24 via <IP_R3_eth0>

# Sur R2 (ajouter b et B) :
ip route add <réseau_b>/24 via <IP_R1_eth0>
ip route add <réseau_B>/24 via <IP_R1_eth0>

# Sur R3 (ajouter a et A) :
ip route add <réseau_a>/24 via <IP_R1_eth1>
ip route add <réseau_A>/24 via <IP_R1_eth1>

# Vérification (depuis chaque routeur, pinger les 4 réseaux) :
ping <IP_réseau_a>
ping <IP_réseau_b>
ping <IP_réseau_A>
ping <IP_réseau_B>
```
**Résultat attendu :** Tous les pings répondent depuis les 3 routeurs.

---

> ⏸️ **POINT D'ARRÊT 1** — Fin section 1.  
> Reprise : refaire mise en place initiale + toute la section 1.

---

## Section 2 — Traceroute

### Mise en place des captures (avant Q1)

```bash
# Sur R1 :
tcpdump -Uni eth0 -w /hosthome/R1_eth0.cap &
tcpdump -Uni eth1 -w /hosthome/R1_eth1.cap &

# Sur la machine physique :
tail -n+0 -f ~/R1_eth0.cap | wireshark -SHkli - &
tail -n+0 -f ~/R1_eth1.cap | wireshark -SHkli - &
```
> ⚠️ Ne jamais arrêter les captures. Si ça plante : fermer Wireshark et relancer la commande `tail...`.  
> Placer eth0 à **gauche**, eth1 à **droite**.

---

### Q1 — Lancer traceroute depuis R2 vers réseau B

```bash
# Sur R2 :
traceroute -q1 -N1 <IP_eth1_R3>
```

---

### Q2 — Que renvoie traceroute ?

Liste des routeurs traversés avec leur IP et temps de réponse en ms.

---

### Q3 — Correspondance des adresses renvoyées

| Ligne | Adresse IP | Routeur | Interface |
|-------|------------|---------|-----------|
| 1 | | R1 | eth0 (côté a) |
| 2 | | R3 | eth1 (côté B) |

---

### Q4 — TTL du 1er paquet envoyé par R2

**Observer dans Wireshark (eth0 de R1)**  
**Résultat attendu :** TTL = **1**

---

### Q5 — Contenu du 1er paquet

**Résultat attendu :** Sonde **UDP** (port destination élevé, ex: 33434+)

---

### Q6 — Ce paquet parvient-il à destination ?

**Non** — TTL = 1 décrémenté à 0 par R1 → R1 jette le paquet.

---

### Q7 — Qui répond ? Quel message ?

| Champ | Valeur |
|-------|--------|
| Qui répond | R1 |
| Protocole | ICMP |
| Type | 11 |
| Code | 0 |
| Signification | Time to Live exceeded in transit |

---

### Q8 — TTL du 2ème paquet

**Résultat attendu :** TTL = **2**

---

### Q9 — Ce paquet parvient-il à destination ?

**Non** — TTL expire chez R3 → paquet jeté.

---

### Q10 — Qui répond ? Quel message ?

| Champ | Valeur |
|-------|--------|
| Qui répond | R3 |
| Protocole | ICMP |
| Type | 11 |
| Code | 0 |
| Signification | Time to Live exceeded in transit |

---

### Q11 — Messages permettant à traceroute de connaître les IPs des routeurs

Les messages **ICMP Type 11 (Time Exceeded)** — l'adresse **source** de ces messages révèle l'IP du routeur intermédiaire.

---

### Q12 — Pourquoi l'IP de eth0 de R3 n'apparaît pas ?

Traceroute affiche l'adresse source des réponses ICMP. La destination finale (eth1 de R3) répond par **ICMP Type 3 Port Unreachable** depuis eth1, et non depuis eth0.

---

> ⏸️ **POINT D'ARRÊT 2** — Fin section 2.  
> Reprise : mise en place initiale + section 1 + relancer les captures Wireshark.

---

## Section 3 — Tracepath

### Q1 — Configurer MTU à 1000 sur la liaison b

```bash
# Sur R1 :
ip link set dev eth1 mtu 1000

# Sur R3 :
ip link set dev eth0 mtu 1000

# Vérification :
ip link show eth1   # sur R1
ip link show eth0   # sur R3
```
**Résultat attendu :** `mtu 1000` visible dans la sortie.

---

### Q2 — Ping 1200 octets avec `-M dont`

```bash
# Sur R2 :
ping -c1 -s 1200 -M dont <IP_réseau_B>
```
**Résultat attendu :** Ping affiché avec fragmentation (paquet découpé par R1 car > MTU 1000).

---

### Q3 — La requête parvient-elle ? Différence avant/après R1 ?

**Oui** — R1 fragmente le paquet (1 paquet → 2 fragments) avant transmission vers R3.  
**Observer dans Wireshark :** 1 paquet sur eth0, 2 fragments sur eth1.

---

### Q4 — Différence sur la réponse ? R1 réassemble-t-il ?

**Non** — un routeur ne réassemble jamais les fragments (seul le destinataire final le fait).  
La réponse de R3 passe fragmentée à travers R1 sans réassemblage.

---

### Q5 — Ping 1200 octets SANS `-M dont`

```bash
# Sur R2 :
ping -c1 -s 1200 <IP_réseau_B>
```
**Résultat attendu :** Echec ou message d'erreur ICMP (Frag needed).

---

### Q6 — La requête parvient-elle ? Flag DF ?

**Non** — flag **DF = 1** positionné par défaut. R1 ne peut pas fragmenter → envoie erreur ICMP.

---

### Q7 — Avertissement de R1 vers R2

| Champ | Valeur |
|-------|--------|
| Protocole | ICMP |
| Type | 3 |
| Code | 4 |
| Signification | Fragmentation Needed and DF was Set |
| Info supplémentaire | Contient le MTU du prochain saut (1000) |

---

### Q8 — Tracepath depuis R2 vers réseau B

```bash
# Sur R2 :
tracepath <IP_réseau_B>
```
**Résultat attendu :** Liste des sauts + détection du MTU (`pmtu 1000` apparaît).

---

### Q9 — Contenu des paquets envoyés par R2

**Résultat attendu :** Paquets **UDP**

---

### Q10 — Valeur du flag DF

**Résultat attendu :** **DF = 1** — tracepath force ce flag pour détecter le Path MTU.

---

### Q11 — Différences dans l'entête IP des paquets successifs

| N° paquet | TTL | Taille |
|-----------|-----|--------|
| 1 | 1 | petite |
| 2 | 2 | petite |
| 3 | 3 | grande (> 1000) |
| 4 | 3 | ≤ 1000 (adapté) |

TTL croissant + taille croissante puis adaptée au MTU détecté.

---

### Q12 — Qui répond aux 2 premiers paquets ?

| Champ | Valeur |
|-------|--------|
| Qui répond | R1 |
| Protocole | ICMP |
| Type | 11 |
| Code | 0 |
| Signification | TTL exceeded |

---

### Q13 — Les 2 premiers paquets arrivent-ils ?

**Non** — TTL expire avant destination (même mécanisme que traceroute).

---

### Q14 — Qui répond au 3ème paquet ?

| Champ | Valeur |
|-------|--------|
| Qui répond | R1 |
| Protocole | ICMP |
| Type | 3 |
| Code | 4 |
| Signification | Fragmentation Needed (paquet trop grand, DF=1) |

---

### Q15 — Le 3ème paquet arrive-t-il ?

**Non** — R1 bloque : paquet > 1000 octets et DF=1, impossible de fragmenter.

---

### Q16 — Qui répond au 4ème paquet ?

| Champ | Valeur |
|-------|--------|
| Qui répond | R3 (destination) |
| Protocole | ICMP |
| Type | 3 |
| Code | 3 |
| Signification | Port Unreachable (paquet arrivé, port UDP fermé) |

---

### Q17 — Le 4ème paquet arrive-t-il ?

**Oui** — tracepath a adapté la taille au MTU détecté (≤ 1000 octets), le paquet passe.

---

### Q18 — Comment tracepath a su que le MTU de b est 1000 ?

Le message **ICMP Type 3 Code 4** renvoyé par R1 contient le champ **Next-Hop MTU = 1000** (RFC 1191).  
Tracepath lit cette valeur et l'affiche dans sa sortie.

---

### Q19 — Résumé du fonctionnement de tracepath

Tracepath envoie des paquets UDP avec **DF=1** et **TTL croissant** + **taille croissante** :
- Les ICMP **Type 11** révèlent les routeurs intermédiaires (comme traceroute)
- Les ICMP **Type 3 Code 4** révèlent les MTU de chaque liaison (Path MTU Discovery)
- Quand le paquet arrive à destination, **ICMP Port Unreachable** termine la trace

---

### Q20 — Similitudes traceroute / tracepath

- Les deux utilisent un **TTL croissant**
- Les deux lisent les **ICMP Type 11** pour identifier les routeurs intermédiaires

---

### Q21 — Différences traceroute / tracepath

| | traceroute | tracepath |
|-|------------|-----------|
| Détection MTU | Non | Oui (DF=1 + ICMP Type 3 Code 4) |
| Taille des paquets | Fixe | Croissante |
| Info affichée | Routeurs + latences | Routeurs + latences + **pmtu** |

---

> ⏸️ **POINT D'ARRÊT 3** — Fin section 3.  
> Reprise : mise en place initiale + section 1 + configurer MTU + captures Wireshark.

---

## Section 4 — RIP

### Mise en place (avant Q1)

```bash
# Supprimer les routes statiques sur chaque routeur :
ip route del <réseau>/24

# Sur chaque routeur, lancer Quagga/Zebra :
ls /var/run/frr/zebra.pid  
/usr/lib/frr/zebra -d
/usr/lib/frr/ripd -d  
vtysh         
```

> Vérifier que les captures Wireshark sont actives.  
> Appliquer le filtre d'affichage **`rip`** dans Wireshark.

---

### Q1 — Activer RIP sur réseau a depuis R1

```
# Dans vtysh sur R1 :
configure terminal
router rip
network 211.230.193.0/26
exit
```
**Résultat attendu :** Messages **RIP Request** visibles dans Wireshark sur eth0 de R1.

---

### Q2 — Couche OSI, protocole, ports, adresse destination

| Champ | Valeur |
|-------|--------|
| Couche OSI | 7 (Application) |
| Protocole transport | UDP |
| Port source | 520 |
| Port destination | 520 |
| Adresse IP destination | 224.0.0.9 (multicast RIPv2) |
| Signification adresse | Groupe multicast de tous les routeurs RIP |

---

### Q3 — Command RIP et version

| Champ | Valeur |
|-------|--------|
| Command | Request (1) |
| Version | 2 |

Ce message demande aux voisins de partager leurs routes.

---

### Q4 — Activer RIP sur réseau b, fréquence des messages

```
# Dans vtysh sur R1 :
configure terminal
router rip
network <réseau_b>/24
exit
```
**Résultat attendu :** Messages **RIP Response** toutes les **30 secondes** (unsolicited updates).

---

### Q5 — Contenu des réponses RIP sur a et sur b

- Sur **a** → R1 annonce le réseau **b** (split horizon : pas d'annonce du réseau reçu sur la même interface)
- Sur **b** → R1 annonce le réseau **a**

---

### Q6 — Activer RIP sur R2 et R3

```
# Sur R2 (vtysh) :
configure terminal
router rip
network <réseau_a>/24
network <réseau_A>/24
exit

# Sur R3 (vtysh) :
configure terminal
router rip
network <réseau_b>/24
network <réseau_B>/24
exit
```
**Intérêt du message de requête :** Obtenir les routes immédiatement sans attendre les 30s d'intervalle.

---

### Q7 — Différences entre réponses RIP de R1 et sa table de routage

```
# Dans vtysh :
show ip route
```
**Différence :** R1 n'annonce pas dans ses réponses les routes apprises depuis l'interface d'envoi (**split horizon** — mécanisme anti-boucle RIP).

---

### Q8 — Métriques

| Annonce | Réseau A | Métrique |
|---------|----------|----------|
| R2 | A | 1 (connecté direct) |
| R1 | A | 2 (1 saut depuis R2) |
| R3 | A | 3 (2 sauts via R1) |

**La métrique = nombre de sauts (hop count)**

---

### Q9 — Tables de routage et métriques

```
# Dans vtysh sur chaque routeur :
show ip route
```

Si lien direct R2-R3 ajouté → métrique de A sur R3 = **2** (directement via R2).

---

### Q10 — Simuler panne eth1 de R2

```bash
# Sur R2 :
ip link set dev eth1 down
```

| Champ | Valeur |
|-------|--------|
| Métrique annoncée pour A | **16** (infini RIP) |
| Signification | Réseau inaccessible (poison reverse) |
| Délai avant suppression dans R3 | ~180 secondes |

---

### Q11 — Désactiver eth0 de R2 — timeout R1

```bash
ip link set dev eth1 up    # remettre eth1
ip link set dev eth0 down  # désactiver eth0
```
**Résultat attendu :** Au bout de **~180 secondes** sans mise à jour, R1 considère A inaccessible (timeout RIP).

---

### Q12 — Routeur PIRATE et attaque RIP

```bash
# Remettre eth0 de R2 :
ip link set dev eth0 up

# Lancer PIRATE :
vstart -D PIRATE --eth0=A

# Sur PIRATE :
ip address add <IP_dans_A>/24 dev eth0
ip link set dev eth0 up
ping <IP_R2>                                          # vérification

# Créer interface fictive dummy0 avec IP dans réseau B :
ifconfig dummy0 <IP_dans_B> netmask 255.255.255.0 up

# Lancer RIP sur PIRATE :
/usr/lib/frr/zebra -d
/usr/lib/frr/ripd -d
vtysh
configure terminal
router rip
network <réseau_A>/24
exit
```

```
# Vérifier table de R2 :
show ip route
```

**Constat :** R2 a une route vers B apprise depuis PIRATE → **usurpation de route**.  
**Remède :** Authentification MD5 sur les échanges RIP ou filtres de routes.

---

> ⏸️ **POINT D'ARRÊT 4** — Fin section 4.  
> Reprise : tout depuis le début.

---

## Section 5 — NAT (bonus)

### Mise en place

```bash
# Arrêter les routeurs en cours, puis :
vstart -D box --eth0=L2 --eth1=L1
vstart -D PC --eth0=L1
vstart -D R-FAI --eth0=L2
```

---

### Q1 — Plan d'adressage privé réseau interne

Exemple : réseau interne L1 = **192.168.1.0/24**

```bash
# Sur PC :
ip address add 192.168.1.2/24 dev eth0
ip link set dev eth0 up

# Sur box (eth1) :
ip address add 192.168.1.1/24 dev eth1
ip link set dev eth1 up
```

---

### Q2 — Réseau public réseau externe

Exemple : réseau externe L2 = **203.0.113.0/24**

```bash
# Sur box (eth0) :
ip address add 203.0.113.1/24 dev eth0
ip link set dev eth0 up

# Sur R-FAI :
ip address add 203.0.113.254/24 dev eth0
ip link set dev eth0 up
```

---

### Q3 — Configurer NAT sur box

```bash
echo 1 > /proc/sys/net/ipv4/ip_forward
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
```

---

### Q4 — Routes nécessaires

```bash
# Sur PC (passerelle = box) :
ip route add default via 192.168.1.1

# Sur R-FAI (route retour) :
ip route add default via 203.0.113.1
```

---

### Q5 — Captures et analyse NAT

```bash
# Sur box :
tcpdump -Uni eth0 -w /hosthome/box_eth0.cap &
tcpdump -Uni eth1 -w /hosthome/box_eth1.cap &

# Sur machine physique :
tail -n+0 -f ~/box_eth0.cap | wireshark -SHkli - &
tail -n+0 -f ~/box_eth1.cap | wireshark -SHkli - &

# Ping depuis PC :
ping -c3 203.0.113.254
```

| Observation | Adresse |
|-------------|---------|
| Source paquet reçu de PC | 192.168.1.2 (IP privée) |
| Source paquet envoyé vers R-FAI | 203.0.113.1 (IP publique box) |
| Destination réponse de R-FAI | 203.0.113.1 |
| Adresse réponse box → PC | 192.168.1.2 |

**Intérêt NAT :** Permet à des machines avec IPs privées d'accéder à Internet via une seule IP publique. Masque la topologie interne.

---

### Q6 — Redirection port HTTP vers PC

```bash
# Sur PC :
nc -lp 8080

# Sur box (DNAT) :
iptables -t nat -A PREROUTING -i eth0 -p tcp --dport 80 -j DNAT --to-destination 192.168.1.2:8080
```

---

### Q7 — Vérification depuis R-FAI

```bash
nmap 203.0.113.1 -p 80
telnet 203.0.113.1 80
# Quitter telnet : Ctrl+AltGr+]  puis  quit
```
**Résultat attendu :** Port 80 ouvert dans nmap, connexion telnet aboutit au `nc` du PC.

---

## Récapitulatif des points d'arrêt

| # | Après | Ce qu'il faut refaire si reprise |
|---|-------|----------------------------------|
| ⏸️ 1 | Q1.4 | Mise en place initiale + section 1 |
| ⏸️ 2 | Q2.12 | Mise en place + section 1 + captures Wireshark |
| ⏸️ 3 | Q3.21 | Mise en place + section 1 + MTU + captures |
| ⏸️ 4 | Q4.12 | Tout depuis le début |

---

*ULILLE-FST-FIL-L3S6-RSX2 — X. Buche*

# 🎮 VIBRASENS_vibreur

**VIBRASENS_vibreur** est le module vibrant sans fil du système **VIBRASENS**, un dispositif de stimulation vibratoire destiné aux enfants en situation de polyhandicap.  
Ce module vibrant est **piloté à distance** via une **connexion Wi-Fi** par le VIBRASENS_controleur (https://github.com/lucas-crunch/VIBRASENS_controleur)

---

## 🧰 Composition Matérielle

### 🔩 Une pochette en tissu

- 13x35cm de tissu de votre choix
- 70cm d’élastique boutonnière
- 4 boutons

---

### ⚡ Composants électroniques

| Composant                | Référence produit | Lien |
|--------------------------|-------------------|------|
| Microcontrôleur          | M5StickC Plus 2   | [🔗 M5Stack](https://shop.m5stack.com/products/m5stickc-plus2-esp32-mini-iot-development-kit) |
| Moteur vibrant           | M5Vibration motor unit | [🔗 M5Stack](https://shop.m5stack.com/products/scroll-unit-with-hollow-shaft-encoder-ec10e1220501](https://shop.m5stack.com/products/vibration-motor-unit)) |
| Câbles de connexion      | Grove M/M (1×5 cm) | [🔗 M5Stack](https://shop.m5stack.com/products/4pin-buckled-grove-cable) |

---

## 🧾 Instructions de Montage

Les instructions de montage se trouve dans **documents_techniques** et se résume en : 
- Programmation de la carte M5StickC2
- Création de la pochette en tissu

---

## 💻 Instructions de Programmation

Les instructions de programmation se trouve dans **documents_techniques** et se résume en : 
- Télécharger les différents logiciels (VSCode, Git, etc.)
- Installer les extensions (PlatformIO)
- Téléverser le code au M5StickC

---

## 💻 Instructions de création de la pochette en tissu

Les instructions pour la création de la pochette en tissu sont disponible dans **documents_techniques** et se résume en : 
- Découpe du tissu
- Couture
- Mise en place des boutons
- Installation de l'électronique à l'intérieur

---

## 🔄 Schéma de la Machine à États

![image](https://github.com/user-attachments/assets/d8961ceb-379e-40cf-b3de-bb2fec11b7af)

--- 

## 📂 Arborescence du dépôt

```bash
VIBRASENS_controller/
├── firmware/               # Code source Arduino/ESP-IDF
├── documents_techniques/            # Documentation technique
├── photos/                 # Images pour la notice de montage
└── README.md

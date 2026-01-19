/*------------------------------------------------------------------------
  Example sketch for Adafruit Thermal Printer library for Arduino.
  Demonstrates a few text styles & layouts, bitmap printing, etc.

  IMPORTANT: DECLARATIONS DIFFER FROM PRIOR VERSIONS OF THIS LIBRARY.
  This is to support newer & more board types, especially ones that don't
  support SoftwareSerial (e.g. Arduino Due).  You can pass any Stream
  (e.g. Serial1) to the printer constructor.  See notes below.
  ------------------------------------------------------------------------*/

#include "Adafruit_Thermal.h"

#include <MIDI.h>

const byte imageBeginIdx = 5;
const int imageMaxSize = 192 * 3;
const byte midiChannel = 15;
const byte imageStartControl = 60;
const byte imageEnd = 50;

bool isImageMode = false;

int imgWriteIndex = 0;

bool imageReceivedMode = false;

struct ArtistText {
    const char* bio;
    const char* link;
};

ArtistText ymnk_ = {"YMNK incarne une musique\nélectronique organique et\ndéjantée où sa casquette\nd'ingénieur-bricoleur est mise\nau service de sa création. Seul\nsur scène, armé de ses claviers\nmodulaires fait maison et d'une\nguitare électrique aux airs\nhéroïques, le\nmulti-instrumentiste propose un\nlive de haute volée avec un\nset-up à son image qui ne laisse\npersonne indifférent.", "https://www.instagram.com/ymnkmusic"};
ArtistText douran_ = {"Douran tisse un dialogue subtil\nentre musique orchestrale et\nélectronique, créant une\nexpérience émotionnelle, intense\net cinématographique. Ses\ncompositions mêlent pianos,\ncordes et synthétiseurs avec\nfinesse pour façonner un univers\nà la fois épique, délicat et\npuissant. Après 4 EPs, plus de\n10 millions d'écoutes, des\nmusiques à l'image, l'artiste\ndévoile un double EP de 14\ntitres : 7 compositions\noriginales avec chœur bulgare,\nquatuor à cordes et synthés\nanalogiques, et 7 remixes.\nSortie finale le 13 mars au\nHasard Ludique à Paris, avec un\nlive inédit accompagné d'un\nbatteur.", "https://open.spotify.com/intl-fr/artist/3Rvdn0CASBunaYXQHEKIoQ"};
ArtistText deadChic_ = {"Une voix de velours éraillée,\nune guitare à la mélodie\nrugueuse, l'alliance rock\nfranco-anglaise que symbolise\nDead Chic est d'une classe\nsubtile et sauvage. Le groupe\namené par Andy Balcon\n(ex-Heymoonshaker) et Damien\nFélix livre des compositions\nriches et généreuses pour un\nlive où les 4 comparses\ndéroulent une folie rock\ncinématographique et\nincantatoire. La proximité du\ngroupe avec le public nous\nplonge dans un univers bien à\neux, à la fois sombre et chic !", "https://open.spotify.com/artist/4hCjCdMZF65nKTdjsPUvfh"};
ArtistText nouvelleVague_ = {"Nouvelle Vague est un groupe\nfrançais formé en 2003 par Marc\nCollin et Olivier Libaux. Il\nréinvente le punk, le post-punk\net la new wave des années 70-80\nen versions acoustiques et bossa\nnova, avec des reprises chantées\nprincipalement par des voix\nféminines. Leur style\nminimaliste, rétro et élégant\ntransforme des classiques en\nsons doux et atmosphériques,\nséduisant un public\ninternational. Connus pour leur\napproche artistique, ils ont\nmarqué la scène française et\nmondiale par cette fusion\naudacieuse de nostalgie et\nmodernité. Nouvel album en 2027", "https://open.spotify.com/artist/4h7NLIlg1oYdEtfQJfyto0"};
ArtistText delaurentis_ = {"Souvent qualifiée\n d'\"électronicienne\", \nDeLaurentis dépasse\nlargement cette\nétiquette. Avec Musicalism, son\nalbum le plus abouti, elle mêle\nexigence savante et énergie\ndansante. Elle explore une\nmusique où sons, couleurs et\némotions fusionnent par la\nsynesthésie. Entourée de\ncollaborations fortes et d'une\nlutherie digitale innovante\n(Erae, IA, gants connectés),\nDeLaurentis compose une musique\ndu futur, immersive et\nprofondément sensible. Le 15\navril, l'artiste se produira au\nPrintemps de Bourges pour une\ncréation spéciale célébrant les\n50 ans du festival, mettant en\nlumière les pionnières de la\nmusique électronique.", "https://open.spotify.com/intl-fr/artist/1Km7DPrZEzlSllBzzkCSN8"};
ArtistText sonia_ = {"Entrez dans SON'IA 360,\nl'expérience électro qui vous\nentoure totalement. DOURAN,\nDELAURENTIS et YMNK combinent\nstéréo, A/V 360, et 3D , avec\nHervé Déjardin de Radio France\naux mix spatial pour faire\nvibrer chaque spectateur. Plus\nqu'un concert, un voyage\nmultidimensionnel où chaque\nnote, chaque mouvement crée une\nconnexion intime et collective,\nredéfinissant le live comme\njamais. Nous fournissons les 12\npoints de diffusion, en\npartenariat avec Nexo de Yamaha.", "https://www.france.tv/spectacles-et-culture/6776809-delaurentis-live-360-a-la-gaite-lyrique.html"};
ArtistText ilEstVilaine_ = {"Quatre ans après Les Mystères de\nLorient, Il Est Vilaine signe\nson retour avec Best Service, un\nalbum foisonnant et cosmopolite\nattendu le 18 septembre 2026.\nDans une époque assombrie, le\ngroupe élargit les horizons et\nfaçonne un univers sonore\nhybride, où se croisent\nélectronique analogique, techno,\nrock vintage et synth-pop. Ces\npaysages intenses, traversés\nd'énergie brute, de groove et\nd'un romantisme sombre, offrent\nun terrain de jeu idéal à des\ncollaborations aussi audacieuses\nque inattendues. Release party :\n13 octobre 2026 - La\nMaroquinerie (Paris).", "https://open.spotify.com/intl-fr/artist/7mUEGrKLGWDJFXX9odcxMM"};
ArtistText prettyInside_ = {"Pretty Inside est un groupe\nbordelais mené par Alexis\nDeux-Seize (membre actif du\ncollectif Flippin' Freaks et\nbassiste d'Opinion). Le groupe\nalterne des brûlots rock garage\net indus, des perles pop\nshoegaze, et un songwriting\nexistentiel et cathartique\ninvoquant des références\néminemment 90's comme plus\nactuelles dont l'homogénéité\nréside dans un chant honnête et\npuissant de spontanéité. Le\nnouvel album du groupe Ever\nGonna Heal sorti le 12 décembre\n2025, est fruit d'un long et\néprouvant processus humain,\namenant le groupe vers une\nidentité singulière et assumée.", "https://open.spotify.com/intl-fr/artist/0NUfUzPh4iA8KZWaEP8MH0"};
ArtistText elvett_ = {"Elvett propose une formation\nautour d'un piano à queue,\nportée par des voix féminines.\nAncré dans une pop exigeante, le\nsextet mêle sons électroniques\net analogiques, chants\ntraditionnels et piano lyrique\nou tribal, pour un voyage\nintrospectif et intense.\nVéritable écrin pour la voix\nsolaire de Lyn M., il explore\nune soul authentique tout en\ninnovant grâce aux productions\nlumineuses d'Alain Frey. Le\nnouvel album sortira le 26\nfévrier 2027, un spectacle\norchestral avec cuivres sera\nprésenté en collaboration avec\nvos conservatoires.", "https://open.spotify.com/intl-fr/artist/4jTcePhvBRxrOONkmpm5WX"};
ArtistText bran_ = {"Sur scène, BV3 Elektrio revisite\nles morceaux phares du\nrépertoire Bran Van, insufflant\nune énergie moderne et dansante\nà des classiques qui n’ont rien\nperdu de leur pouvoir\nrassembleur. Mais la proposition\nne se limite pas à la nostalgie\n: de toutes nouvelles\ncompositions, jusqu'ici\nsoigneusement gardées secrètes\npar Di Salvio, viendront\nenrichir le spectacle. Le\nrésultat : un live incandescent,\nporté par des grooves\nhypnotiques, des nappes\nélectroniques et une complicité\nmusicale qui rappellent combien\nBran Van 3000 a toujours su\ntraverser les époques.", "https://open.spotify.com/artist/3D3blX3lLE3BCMdo3SOMlB"};
ArtistText vss_ = {"Vipères Sucrées Salées crache un\nrock nerveux et tranchant, riffs\nacérés, énergie brute et textes\nen français qui frappent. Chaque\nlive est un uppercut sonore, un\nmaelström électrique qui emporte\nle public. Du trio au quintet,\nil prépare déjà la suite,\npromettant de nouveaux morceaux\naussi intenses et captivants que\nleur premier chaos sonore.", "https://open.spotify.com/intl-fr/album/2c6Y0gdj5J23TeQhEHJwq2"};

unsigned char show[] = {
  0x1B, 0x2A, 0x20, 0xC0, 0x00,  //Bitmap size: 24*192, revise parameter with the reference of general instruction set 34 and 35.

  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,  //--------------
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0xff, 0xff, 0xff,  //--------------
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,  //--------------
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0xff, 0xff, 0xff,  //--------------
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,  //--------------
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,  //--------------
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,  //--------------
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0xff, 0xff, 0xff,  //--------------
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0xff, 0xff, 0xff,
  0x00, 0x00, 0x00,
  0xff, 0xff, 0xff,
  0x00, 0x00, 0x00,
  0xff, 0xff, 0xff,
  0x00, 0x00, 0x00,
  0xff, 0xff, 0xff,

  0x1B, 0x40,

  0x1B, 0x33, 0x00,
  //0x0A,                                                                                                                              //Print bitmap instruction

};

MIDI_CREATE_DEFAULT_INSTANCE();

// Here's the new syntax when using SoftwareSerial (e.g. Arduino Uno) ----
// If using hardware serial instead, comment out or remove these lines:

#include "SoftwareSerial.h"
#define TX_PIN 6  // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN 5  // Arduino receive   GREEN WIRE   labeled TX on printer

SoftwareSerial mySerial(RX_PIN, TX_PIN);  // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);      // Pass addr to printer constructor
// Then see setup() function regarding serial & printer begin() calls.

// Here's the syntax for hardware serial (e.g. Arduino Due) --------------
// Un-comment the following line if using hardware serial:

//Adafruit_Thermal printer(&Serial1);      // Or Serial2, Serial3, etc.

// -----------------------------------------------------------------------

void printQR(const char* text) {
  uint8_t len = strlen(text) + 3;

  mySerial.write(0x1D);
  mySerial.write(0x28);
  mySerial.write(0x6B);
  mySerial.write((byte)len);
  mySerial.write((byte)0x00);
  mySerial.write(0x31);
  mySerial.write(0x50);
  mySerial.write(0x30);
  mySerial.print(text);

  mySerial.write(0x1B);
  mySerial.write(0x61);
  mySerial.write(0x01);
  mySerial.write(0x1D);
  mySerial.write(0x28);
  mySerial.write(0x6B);
  mySerial.write(0x03);
  mySerial.write((byte)0x00);
  mySerial.write(0x31);
  mySerial.write(0x51);
  mySerial.write(0x30);
}

void printAsciiFun(const char* txt) {
  int count = 0;
  while (*txt) {
    uint8_t c = (uint8_t)*txt++;

    // UTF-8 accented characters start with 0xC3
    if (c == 0xC3 && *txt) {
      uint8_t next = (uint8_t)*txt++;

      switch (next) {

        // é è ê ë
        case 0xA9: // é
        case 0xA8: // è
        case 0xAA: // ê
        case 0xAB: // ë
          printer.write('&');
          break;

        // à â ä
        case 0xA0: // à
        case 0xA2: // â
        case 0xA4: // ä
          printer.write('@');
          break;

        // ù û ü
        case 0xB9: // ù
        case 0xBB: // û
        case 0xBC: // ü
          printer.write('_');
          break;

        // ô ö
        case 0xB4: // ô
        case 0xB6: // ö
          printer.write('0');
          break;

        // î ï
        case 0xAE: // î
        case 0xAF: // ï
          printer.write('!');
          break;

        // ç
        case 0xA7: // ç
          printer.write('(');
          break;

        // œ (UTF-8 : C5 93)
        default:
          // on ignore ici, traité plus bas
          break;
      }
    }

    // œ
    else if (c == 0xC5 && *txt == 0x93) {
      txt++;
      printer.print("oe");
    }

    // æ
    else if (c == 0xC3 && *txt == 0xA6) {
      txt++;
      printer.print("ae");
    }

    // ASCII normal
    else {
      printer.write(c);
    }
  }
}


void setup() {

  mySerial.begin(9600);  // Initialize SoftwareSerial

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleProgramChange(handleProgramChange);
  // put your setup code here, to run once:
  MIDI.begin(midiChannel);

  while (mySerial.read() >= 0) {}
/*
  mySerial.write(show, sizeof(show));
  mySerial.write(show, sizeof(show));
  mySerial.write(show, sizeof(show));
  */
  printer.feed(2);

  // Select code page CP1252 (Latin-1 occidental)
mySerial.write(0x1B);
mySerial.write(0x74);
mySerial.write(0x10); // 0x10 = CP1252 (selon firmware)

  printArtist(&ymnk_);
  delay(10);
  printArtist(&douran_);
  delay(10);
  printArtist(&deadChic_);
  delay(10);
  printArtist(&nouvelleVague_);
  delay(10);
  printArtist(&delaurentis_);
  delay(10);
  printArtist(&sonia_);
  delay(10);
  printArtist(&ilEstVilaine_);
  delay(10);
  printArtist(&elvett_);
  delay(10);
  printArtist(&bran_);
  delay(10);
  printArtist(&vss_);
}


void handleControlChange(byte channel, byte control, byte value) {
  if (control == imageStartControl) {
    isImageMode = true;
    imageReceivedMode = !imageReceivedMode;
    if (imageReceivedMode) {
      imgWriteIndex = 0;
    } else {
      mySerial.write(show, sizeof(show));
    }
    digitalWrite(LED_BUILTIN, imageReceivedMode ? HIGH : LOW);
  } else if (control == imageEnd) {
    printer.feed(3);
    isImageMode = false;
    
  }
}

void handleProgramChange(byte channel, byte program) {

}

void handleNoteOn(byte channel, byte note, byte velocity) {
  if (imageReceivedMode) {

    uint8_t value = note & 0x7F;

    if (velocity == 1) {
        value |= 0x80;
    }
    
    show[imageBeginIdx + imgWriteIndex] = value;
     
    //show[imageBeginIdx + imgWriteIndex] = (note > 60) ? 0xff : 0x00;
    imgWriteIndex++;
   
    if (imgWriteIndex >= imageMaxSize) {
      imgWriteIndex = 0;
    }
  } else if (!isImageMode) {
    //printThatShit();
  }
}

void handleNoteOff(byte channel, byte note, byte velocity) {
}

void printArtist(ArtistText* artist) {
  // Font options
  printer.setFont('B');

  //mySerial.println();
  mySerial.print("--------------------------------");
  printAsciiFun(artist->bio);
  printer.println();
  delay(5);
  printer.feed(1);
  printQR(artist->link);
  mySerial.print("--------------------------------");
  printer.feed(1);
  printer.sleep();       // Tell printer to sleep
  delay(1000L);          
  printer.wake();        // MUST wake() before printing again, even if reset
  printer.setDefault();  // Restore printer to defaults
}


void loop() {
  // put your main code here, to run repeatedly:
  MIDI.read();
}

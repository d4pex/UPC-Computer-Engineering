(define (domain locks-keys)
  (:requirements :strips :typing :negative-preconditions)
  (:types AGENT TCASELLA CLAU PORTA)

  (:predicates
    ;; Mapa i estat
    (adj ?x - TCASELLA ?y - TCASELLA) ; adjacència entre caselles veïnes
    (AgentEn ?a - AGENT ?c - TCASELLA) ; posició de cada agent
    (ClauEn  ?k - CLAU  ?c - TCASELLA) ; la clau es troba en aquesta casella
    (PortaEn ?d - PORTA ?c - TCASELLA) ; la porta ocupa aquesta casella
    (EsSortida ?c - TCASELLA) ; casella de sortida
    (Emparella ?d - PORTA ?k - CLAU) ; quina clau obre quina porta

    ;; Estat global de claus (compartides per tots els agents)
    (TeClau ?k - CLAU) ; clau ja recollida
  )

  ;; 1 Moure: desplaçar-se a una casella connectada que NO sigui una porta ni tingui clau
  (:action moure
    :parameters (?a - AGENT ?from - TCASELLA ?to - TCASELLA)
    :precondition (and
      (AgentEn ?a ?from)
      (adj ?from ?to)
      (forall (?d - PORTA) (not (PortaEn ?d ?to))) ;; no hi ha porta
      (forall (?k - CLAU) (not (ClauEn ?k ?to)))   ;; no hi ha clau
    )
    :effect (and
      (AgentEn ?a ?to)
      (not (AgentEn ?a ?from))
    )
  )

  ;; 2 Moure-agafant-clau: moure’s a una casella i agafar automàticament la clau si hi ha
  (:action moure-agafant-clau
    :parameters (?a - AGENT ?from - TCASELLA ?to - TCASELLA ?k - CLAU)
    :precondition (and
      (AgentEn ?a ?from)
      (adj ?from ?to)
      (ClauEn ?k ?to)                   ;; hi ha la clau a la casella de destí
      (forall (?d - PORTA) (not (PortaEn ?d ?to))) ;; no hi ha porta
    )
    :effect (and
      (AgentEn ?a ?to)
      (not (AgentEn ?a ?from))
      (TeClau ?k)
      (not (ClauEn ?k ?to))
    )
  )

  ;; 3 EntrarPorta: permet entrar a una casella amb porta si es té la clau correcta
  (:action entrar-porta
    :parameters (?a - AGENT ?from - TCASELLA ?to - TCASELLA ?d - PORTA ?k - CLAU)
    :precondition (and
      (AgentEn ?a ?from)
      (adj ?from ?to)
      (PortaEn ?d ?to)
      (Emparella ?d ?k)
      (TeClau ?k)
    )
    :effect (and
      (AgentEn ?a ?to)
      (not (AgentEn ?a ?from))
    )
  )
)

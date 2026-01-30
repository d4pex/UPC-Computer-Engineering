(define (problem mapA)
    (:domain locks-keys)
    (:objects
        a1 a2 - AGENT
        C1x1 C1x2 C1x3 C1x4 C1x5
        C2x1 C2x2 C2x3 C2x4 C2x6
        C3x1 C3x2 C3x3 C3x4 C3x5 C3x6 - TCASELLA
        clauA - CLAU
        portaA - PORTA
    )
    (:init
        ;; Estat inicial

        ;; Agents
        (AgentEn a1 C3x4) ;; Agent a1 a la casella C3x4
        (AgentEn a2 C1x2) ;; Agent a2 a la casella C1x2

        ;; Clau i porta
        (ClauEn clauA C2x1) ;; Clau clauA a la casella C2x1
        (PortaEn portaA C3x6) ;; Porta portaA a la casella C3x6

        ;; Connexió clau-porta
        (Emparella portaA clauA) ;; La clau clauA obre la porta portaA

        ;; Sortida
        (EsSortida C2x6) ;; Casella de sortida

        ;; Connexions horitzontals
        ;; Fila 1
        (adj C1x1 C1x2) (adj C1x2 C1x1)
        (adj C1x2 C1x3) (adj C1x3 C1x2)
        (adj C1x3 C1x4) (adj C1x4 C1x3)
        (adj C1x4 C1x5) (adj C1x5 C1x4)

        ;; Fila 2
        (adj C2x1 C2x2) (adj C2x2 C2x1)
        (adj C2x2 C2x3) (adj C2x3 C2x2)
        (adj C2x3 C2x4) (adj C2x4 C2x3)

        ;; Fila 3
        (adj C3x1 C3x2) (adj C3x2 C3x1)
        (adj C3x2 C3x3) (adj C3x3 C3x2)
        (adj C3x3 C3x4) (adj C3x4 C3x3)
        (adj C3x4 C3x5) (adj C3x5 C3x4)
        (adj C3x5 C3x6) (adj C3x6 C3x5)

        ;; Connexions verticals
        ;; Columna 1
        (adj C1x1 C2x1) (adj C2x1 C1x1)
        (adj C2x1 C3x1) (adj C3x1 C2x1)

        ;; Columna 2
        (adj C1x2 C2x2) (adj C2x2 C1x2)
        (adj C2x2 C3x2) (adj C3x2 C2x2)

        ;; Columna 3
        (adj C1x3 C2x3) (adj C2x3 C1x3)
        (adj C2x3 C3x3) (adj C3x3 C2x3)

        ;; Columna 4
        (adj C1x4 C2x4) (adj C2x4 C1x4)
        (adj C2x4 C3x4) (adj C3x4 C2x4)

        ;; Columna 5 (no té connexions)

        ;; Columna 6
        (adj C2x6 C3x6) (adj C3x6 C2x6)

    )
    (:goal
        ;; L'objectiu és que qualsevol agent arribi a la sortida
        (or (AgentEn a1 C2x6) (AgentEn a2 C2x6))
    )
)

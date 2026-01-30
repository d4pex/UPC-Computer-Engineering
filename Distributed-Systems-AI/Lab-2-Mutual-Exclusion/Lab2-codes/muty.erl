-module(muty).
-export([start/3, start/4, stop/0, stop/1]).

start(Lock, Sleep, Work) ->
    register(l1, apply(Lock, start, [1])),
    register(l2, apply(Lock, start, [2])),
    register(l3, apply(Lock, start, [3])),
    register(l4, apply(Lock, start, [4])),
    l1 ! {peers, [l2, l3, l4]},
    l2 ! {peers, [l1, l3, l4]},
    l3 ! {peers, [l1, l2, l4]},
    l4 ! {peers, [l1, l2, l3]},
    register(w1, worker:start("John", l1, Sleep, Work)),
    register(w2, worker:start("Ringo", l2, Sleep, Work)),    
    register(w3, worker:start("Paul", l3, Sleep, Work)),
    register(w4, worker:start("George", l4, Sleep, Work)),
    ok.

stop() ->
    w1 ! stop,
    w2 ! stop,
    w3 ! stop,
    w4 ! stop.

start(Lock, Sleep, Work, [N1, N2, N3, N4]) ->
    % Arrancamos los LOCKS en remoto
    % Usamos un rpc:call anidado. El de dentro crea el proceso ALLI. Y el de fuera registra ese proceso ALLI.
    rpc:call(N1, erlang, register, [l1, rpc:call(N1, Lock, start, [1])]),
    rpc:call(N2, erlang, register, [l2, rpc:call(N2, Lock, start, [2])]),
    rpc:call(N3, erlang, register, [l3, rpc:call(N3, Lock, start, [3])]),
    rpc:call(N4, erlang, register, [l4, rpc:call(N4, Lock, start, [4])]),

    % Conectamos los Peers
    % Enviamos la tupla {Nombre, Nodo} para que sepan donde enviar el mensaje
    {l1, N1} ! {peers, [{l2, N2}, {l3, N3}, {l4, N4}]},
    {l2, N2} ! {peers, [{l1, N1}, {l3, N3}, {l4, N4}]},
    {l3, N3} ! {peers, [{l1, N1}, {l2, N2}, {l4, N4}]},
    {l4, N4} ! {peers, [{l1, N1}, {l2, N2}, {l3, N3}]},

    % Arrancamos los WORKERS en remoto
    % Igual que antes, forzamos que el worker nazca ALLI
    rpc:call(N1, erlang, register, [w1, rpc:call(N1, worker, start, ["John", l1, Sleep, Work])]),
    rpc:call(N2, erlang, register, [w2, rpc:call(N2, worker, start, ["Ringo", l2, Sleep, Work])]),
    rpc:call(N3, erlang, register, [w3, rpc:call(N3, worker, start, ["Paul", l3, Sleep, Work])]),
    rpc:call(N4, erlang, register, [w4, rpc:call(N4, worker, start, ["George", l4, Sleep, Work])]),
    ok.

stop([N1, N2, N3, N4]) ->
    {w1, N1} ! stop,
    {w2, N2} ! stop,
    {w3, N3} ! stop,
    {w4, N4} ! stop.
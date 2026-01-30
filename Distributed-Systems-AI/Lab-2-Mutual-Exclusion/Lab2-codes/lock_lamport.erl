-module(lock3).
-export([start/1]).

start(MyId) ->
    spawn(fun() -> init(MyId, 0) end).

init(MyId, Clock) ->
    receive
        {peers, Nodes} ->
            open(Nodes, MyId, Clock);
        stop ->
            ok
    end.

open(Nodes, MyId, Clock) ->
    receive
        {take, Master, Ref} ->
            % Incrementamos reloj antes de solicitar
            NewClock = Clock + 1,
            Refs = requests(Nodes, MyId, NewClock),
            % Pasamos nuestro Timestamp (NewClock, MyId) a wait para comparar
            wait(Nodes, Master, Refs, [], Ref, MyId, NewClock, {NewClock, MyId});
        {request, From, Ref, RemoteClock, _RemoteId} ->
            % Actualizamos reloj al recibir mensaje: max(Local, Remote) + 1
            NewClock = erlang:max(Clock, RemoteClock) + 1,
            From ! {ok, Ref},
            open(Nodes, MyId, NewClock);
        stop ->
            ok
    end.

requests(Nodes, MyId, Clock) ->
    lists:map(
      fun(P) -> 
        R = make_ref(), 
        P ! {request, self(), R, Clock, MyId}, 
        R 
      end, 
      Nodes).

% MyRequestTS es una tupla {Clock, Id} que representa cuándo pedí yo el lock
wait(Nodes, Master, [], Waiting, TakeRef, MyId, Clock, _MyRequestTS) ->
    Master ! {taken, TakeRef},
    held(Nodes, Waiting, MyId, Clock);
wait(Nodes, Master, Refs, Waiting, TakeRef, MyId, Clock, MyRequestTS) ->
    receive
        {request, From, Ref, RemoteClock, RemoteId} ->
            % Actualizamos reloj local
            NewClock = erlang:max(Clock, RemoteClock) + 1,
            
            % Comparamos Timestamps: {RemoteClock, RemoteId} vs {MyClock, MyId}
            % Si el remoto es menor (ocurrio antes), tiene prioridad.
            {MyPrioClock, _} = MyRequestTS,
            
            PriorityRemote = (RemoteClock < MyPrioClock) or 
                             ((RemoteClock == MyPrioClock) and (RemoteId < MyId)),

            if 
                PriorityRemote ->
                    From ! {ok, Ref},
                    wait(Nodes, Master, Refs, Waiting, TakeRef, MyId, NewClock, MyRequestTS);
                true ->
                    wait(Nodes, Master, Refs, [{From, Ref}|Waiting], TakeRef, MyId, NewClock, MyRequestTS)
            end;
            
        {ok, Ref} ->
            NewRefs = lists:delete(Ref, Refs),
            wait(Nodes, Master, NewRefs, Waiting, TakeRef, MyId, Clock, MyRequestTS);
        release ->
            ok(Waiting),            
            open(Nodes, MyId, Clock)
    end.

ok(Waiting) ->
    lists:map(
      fun({F,R}) -> 
        F ! {ok, R} 
      end, 
      Waiting).

held(Nodes, Waiting, MyId, Clock) ->
    receive
        {request, From, Ref, RemoteClock, _RemoteId} ->
            NewClock = erlang:max(Clock, RemoteClock) + 1,
            held(Nodes, [{From, Ref}|Waiting], MyId, NewClock);
        release ->
            ok(Waiting),
            open(Nodes, MyId, Clock)
    end.
# Projekt 2 - Serwer mutexów i condition variables


Należy rozbudować system Minix o serwer implementujący funkcjonalności 'mutexów' oraz 'condition variables'.

Zarówno mutexy jak i condition variables będą identyfikowane w systemie przez liczby typu `int`.
Implementacje opisanych poniżej funkcji powinny zostać dodane do biblioteki systemowej (np. w `lib\libc\sys-minix\`).

### Mutexy
Funkcje:

* `cs_lock(int mutex_id)` - próbuje zarezerwować mutex o numerze przekazanym w argumencie.
Jeśli mutex nie jest w posiadaniu żadnego procesu powinien być przydzielony procesowi który wywołał funkcję.
W takim przypadku funkcja zwraca `0` (sukces).
Jeśli inny proces jest w posiadaniu mutexu bieżący proces powinien być zawieszony aż do momentu kiedy mutex będzie mógł być mu przydzielony.
W przypadku kiedy proces otrzymuje mutex funkcja zwraca `0` (sukces).
Żaden proces nie powinien żądać mutexu który już jest w jego posiadaniu.
Zachowanie w takim przypadku jest niezdefiniowane, z tym że niedopuszczalna jest sytuacja kiedy wskutek takiego działania przestaje działać system lub serwer mutexów.
* `cs_unlock(int mutex_id)` - zwalnia mutex o numerze przekazanym w argumencie.
Jeśli wołający proces jest w posiadaniu mutexu, funkcja zwraca `0` (sukces) a serwer mutexów przydziela mutex następnemu procesowi z kolejki procesów oczkujących na ten mutex (jeśli kolejka nie jest pusta).
Jeśli proces wołający nie jest w posiadaniu tego mutexu funkcja zwróci `-1` i ustawi `errno` na `EPERM`.

Procesy oczekujące na jeden mutex powinny być ustawiane w kolejkę (FIFO).

### Condition variables
Funkcje:

* `cs_wait(int cond_var_id, int mutex_id)` - zawiesza bieżący proces w oczekiwaniu na zdarzenie identyfikowane przez `cond_var_id`.
Proces wywołujący tę funkcję powinien być w posiadaniu mutexu identyfikowanego przez `mutex_id`.
Jeśli proces wołający nie posiada odpowiedniego mutexu funkcja powinna zwrócić `-1` i ustawić `errno` na `EINVAL`.
Jeśli proces wołający jest w posiadaniu mutexu serwer powinien zwolnić mutex i zawiesić wołający proces aż do czasu gdy jakiś inny proces nie ogłosi zdarzenia `cond_var_id` za pomocą funkcji `cs_broadcast`.
W takim przypadku serwer powinien ustawić proces w kolejce procesów oczekujących na  mutex `mutex_id` i po otrzymaniu mutexu zwrócić `0` (sukces).
*	`cs_broadcast(int cond_var_id)` - ogłasza zdarzenie identyfikowane przez `cond_var_id`. Wszystkie procesy które zawiesiły się w oczekiwaniu na to zdarzenie powinny zostać odblokowane. Każdy z nich po odzyskaniu swojego mutexu powinien zostać wznowiony.

Można przyjąć że w każdym momencie działania serwera co najwyżej 1024 mutex'y będą zarezerwowane.

## Sygnały
Przychodzące sygnały powinny być natychmiast obsługiwane zgodnie z zarejestrowanymi przez proces procedurami obsługi.
Powyższe funkcje blokujące nie mogą jednak być przerywane i zwracać `EINTR` w `errno`.
Jeśli proces oczekiwał na mutex, to po obsłudze sygnału powinien wznowić oczekiwanie.
Jeśli oczekiwał na zdarzenie to powinien odzyskać mutex i zwrócić sukces (tzw. spurious wakeup).

Mutex'y procesów które zostają zakończone powinny być natychmiast zwalniane.

## Struktura repozytorium
Rozwiązanie będzie testowane w systemie MINIX 3.2.1.
- `src/` odzwierciedla fragment struktury katalogów źródeł systemu MINIX.
Fragment ten zawiera niektóre katalogi w których, mogą się znaleźć części implementacji.
W celu przetestowania należy skopiować zawartość katalogu `src` z repozytorium do katalogu `/usr/src` w MINIX'e.
Wskutek tego część plików oryginalnych źródeł zostanie nadpisana.
Następnie należy zainstalować include'y, przekompilować i zainstalować bibliotekę standardową oraz nowy serwer.
Aby umożliwić zmiany w podstawowych serwerach systemowych (np. w PM) przed testowaniem zostanie również przekompilowany obraz systemu.
Po restarcie systemu, nowa funkcjonalność powinna działać po uruchomieniu serwera.

- `tests/` zawiera kilka podstawowych testów opisanych powyżej funkcjonalności.

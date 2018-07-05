$TTL	1h
@ 	IN 	SOA 	ns.siklab.mimuw.edu.pl. root.ns.siklab.mimuw.edu.pl. (
		1 	; Serial
		604800 	; Refresh
		86400 	; Retry
		2419200 ; Expire
		604800) ; Negative Cache TTL
;
;
		IN 	NS 	ns
localhost 	IN 	A 	127.0.0.1
ns 		IN 	A       10.10.0.10
H1		IN 	A       10.10.0.10
R1 		IN 	A       10.10.0.1
R1 		IN 	A	10.12.0.1
R2 		IN 	A       10.12.0.2
R2		IN 	A       10.11.0.1
H2 		IN 	A       10.11.0.10 


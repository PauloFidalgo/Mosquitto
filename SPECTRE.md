# Spectre

Test Circuit Setup
```spectre
simulator lang=spectre
// MOSFET models (example)
include "/home/cadmgr/libs/gpdk045/v6.0/models/spectre/gpdk045.scs" section=mc
simOptions options temp=27 save=allpub
Settings options rawfmt=psfbin // for ViVA

global 0
Vss (s 0) vsource dc=0 // reference
Vgs (g s) vsource dc=0 // Gate-source voltage
Vds (d s) vsource dc=0 // Drain-source voltage
Vbs (b s) vsource dc=0 // Bulk-source voltage

M1 (d g s b) g45n1svt w=120n l=45n

sweep_gs sweep dev=Vgs param=dc start=0.3 stop=1 step=0.1 {
    dc1 dc dev=Vds param=dc start=0 stop=1 step=0.01 // VDS sweep
}
save M1:ids // additional information to save
```

## Sweep Command
```spectre
sweep <name1> ... parameters ... {
    sweep <name2> ... parameters ... {
        <analysis specification>
    }
}
```

## DC Analysis Command
```spectre
dc <nameDC> ... parameters ...
```

## Inverter transfer

```spectre
simulator lang=spectre
include "/home/cadmgr/libs/gpdk045/v6.0/models/spectre/gpdk045.scs" section=mc
Settings options rawfmt=psfbin
simOptions options temp=27 save=allpub
global 0 vdd gnd
parameters Supply=1.0

// Supply voltages
Vss (gnd 0) vsource dc=0
Vdd (vdd 0) vsource dc=Supply
Vin (in gnd) vsource dc=0

M2 (out in vdd vdd) g45p1svt w=120n l=45n
M1 (out in gnd gnd) g45n1svt w=120n l=45n

transfer dc param=dc dev=Vin start=0 stop=1 step=0.01
```

### Sweep por power supply value Inverter

```spectre
// initial setup like in the preceeding example
parameters Supply=1.0
global 0 gnd vdd
Vss (gnd 0) vsource dc=0
Vdd (vdd 0) vsource dc=Supply
Vin (in gnd) vsource dc=0

M2 (out in vdd vdd) g45p1svt w=120n l=45n
M1 (out in gnd gnd) g45n1svt w=120n l=45n

supply_sweep sweep param=Supply start=0.8 stop=1.2 step=0.1 {
    transfer dc param=dc dev=Vin start=0 stop=1 step=0.01
}
```

### Sweep in PMOS Width Inverter
```spectre
parameters Supply=1.0 f=1
global 0 gnd vdd
Vss (gnd 0) vsource dc=0
Vdd (vdd 0) vsource dc=Supply
Vin (in gnd) vsource dc=0

M2 (out in vdd vdd) g45p1svt w=f*120n l=45n
M1 (out in gnd gnd) g45n1svt w=120n l=45n

supply_sweep sweep param=f start=1 stop=4 step=1 {
    transfer dc param=dc dev=Vin start=0 stop=1 step=0.01
}
```

# Lab 1

## Encontrar I-V curves de nmos para l=250n e w=1.2u
ivnmos.scs
```
//*I-V curves for MOSFET of type N

// models for this task are in SPICE format
simulator lang=spice
// change as necessary
.lib "cmos250.lib" TT 
simulator lang=spectre

Settings options rawfmt=psfbin
myOpts   options currents=all save=allpub
global 0 

// Set the Supply Voltage as a parameter, so that the code is easier to adapt to other technologies
parameters	Supply = 2.5

//***** Power supply signals
// Vdd (VDD 0) vsource dc=Supply

//***** The source of the transistor is the reference for the voltage
VDS (drain 0) vsource dc=Supply
VGS (gate  0) vsource dc=Supply

//***** Define sweep parameters of VGS and VDS for DC simulation 
swp_vds sweep dev=VGS param=dc start=0 stop=Supply step=0.5 {
	swp_vgs dc dev=VDS param=dc start=0 stop=Supply step=0.1 
}

//***** Device
M0 (drain gate 0 0)	nmos l=250n w=1.2u

save M0:ids
```

## Encontrar logo para nmos pequeno e grande
```
//*I-V curves for MOSFET of type N
// models for this task are in SPICE format
simulator lang=spice
.lib "cmos250.lib" TT 
simulator lang=spectre

Settings options rawfmt=psfbin currents=all save=allpub
parameters	Supply=2.5
global 0 

VDS (drain 0) vsource dc=Supply
VGS (gate  0) vsource dc=Supply 
M0 (drain gate 0 0)	nmos l=0.25u w=1.2u

save M0:ids // save additional info
// First analysis
swp_vds dc dev=VDS param=dc start=0 stop=Supply step=0.01 

// Change the parameters (alteration command is "alter")
largerTrans alter dev=M0 param=l value=0.5u
largerTrans1 alter dev=M0 param=w value=4.8u
swp_vdsL dc dev=VDS param=dc start=0 stop=Supply step=0.01 // run sweep again
```

## Encontrar para pmos, que diferenças tem?
```
swp_vds dc dev=VDS param=dc start=0 stop=-Supply step=-0.01 // Porque o Vds do PMOS é negativo
```

## Encontrar Ids e Vdsat -> Basta fazer um save M0:ids M0:vdsat
```
//*I-V curve for MOSFET of type N, 250nm technology
// models for this task are in SPICE format
simulator lang=spice
// change as necessary
.lib "cmos250.lib" TT 
simulator lang=spectre

Settings options rawfmt=psfbin
myOpts   options currents=all save=allpub
parameters	Supply = 2.5
global 0 

//***** The source of the transistor is the reference for the voltage
VDS (drain 0) vsource dc=Supply
VGS (gate  0) vsource dc=Supply

swp_vgs dc dev=VDS param=dc start=0 stop=Supply step=0.01 

//***** Device
M0 (drain gate 0 0)	nmos l=0.25u w=1.2u
save M0:ids M0:vdsat    // <<= plot Vdsat
```

## Encontrar variação de Ids(Vdd) com diferentes temperaturas, e vdd
```
//*I-V curves for MOSFET of type N

simulator lang=spice
.lib "cmos250.lib" FF      // Processo 
simulator lang=spectre

Settings options rawfmt=psfbin temp=0   // Temperatura 
myOpts   options currents=all save=allpub
parameters	Supply = 2.5*1.1    // Supply Voltage 
global 0 

//***** The source of the transistor is the reference for the voltage
VDS (drain 0) vsource dc=Supply
VGS (gate  0) vsource dc=Supply

swp_vgs dc dev=VDS param=dc start=0 stop=Supply step=0.01 

//***** Device
M0 (drain gate 0 0)	nmos l=0.25u w=1.2u
save M0:ids
```

Operating point: Fast = FF, Slow = SS, Nominal = TT

## Encontrar comportamento na subthreshold region

```
//*Ids(Vgs) curves for MOSFET of type N, 45nm technology, two values of Vdd
simulator lang=spice
.lib "45nm.lib" TT 
simulator lang=spectre

Settings options rawfmt=psfbin
myOpts   options currents=all save=allpub
parameters	Supply = 1.1
global 0 

//***** The source of the transistor is the reference for the voltage
VDS (drain 0) vsource dc=Supply
VGS (gate  0) vsource dc=Supply

//***** Device
M0 (drain gate 0 0)	nmos l=50n w=90n
save M0:ids

swp_vgs dc dev=VGS param=dc start=0 stop=0.5 step=0.01 //sweep of Vgs for Vds=Vdd

lowVDS alter dev=VDS param=dc value=0.1*Supply  // CHANGE
swp_vgsL dc dev=VGS param=dc start=0 stop=0.5 step=0.01 // //sweep of Vgs for Vds=0.1*Vdd
```

# Lab 2

## Encontrar VTC para inversor minimo

```spectre
// Minimum inverter circuit
simulator lang=spice
.lib "cmos250.lib" TT   // library file mmust be in the same folder
simulator lang=spectre

Settings options rawfmt=psfbin
parameters	Supply=2.5 
global 0 VDD

subckt inverter in out
M1 (out in VDD VDD)	PMOS l=250n w=300n
M2 (out in 0 0) 	NMOS l=250n w=300n
ends inverter

//***** Power supply signals
Vdd (VDD 0) vsource dc=Supply
Va  (A 0) vsource dc=0

//***** Instance of the inverter
I1 (A Y) 	inverter

swp_va dc dev=Va param=dc start=0 stop=Supply step=0.01
```

## Equal noise margin (Smallest Balanced Inverter):

```
// Inverter circuit: sweep for PMOS width
simulator lang=spice
.lib "cmos250.lib" TT 
simulator lang=spectre

Settings options rawfmt=psfbin
global 0 VDD
parameters	Wmin=300n
parameters	Supply=2.5

subckt inverter in out
parameters Wp=Wmin     // subcircuit parameter
M1 (out in VDD VDD)	 PMOS l=250n w=Wp
M2 (out in 0 0) 	   NMOS l=250n w=300n
ends inverter

//***** Power supply signals
Vdd (VDD 0) vsource dc=Supply
Va  (A 0) vsource dc=0

//***** Instance of the inverter
I1 (A Y) 	inverter

// **** analysis
swp_wp sweep sub=I1 param=Wp start=Wmin stop=5*Wmin step=0.5*Wmin {
	swp_va dc dev=Va param=dc start=0 stop=Supply step=0.01
}
```

## Medir Propagation Delay do Inversor Balanceado
Para usar isto, devemos alterar o circuito para em vez de vsource user vpulse.
Objetivo: Curvas tp, tphl, tplh como função da Carga (load)

O novo .scs file:
```
// Inverter circuit: delay vs. load
simulator lang=spice
.lib "cmos250.lib" TT 
simulator lang=spectre

Settings options rawfmt=psfbin
global 0 VDD
parameters	Wmin=300n
parameters	Supply=2.5 load=1f
parameters  pulse_delay=1ns rise_time=100ps fall_time=100ps high_time=2ns perd=4.2ns

subckt inverter in out
M1 (out in VDD VDD)	 PMOS l=250n w=1050n  // determined in task 2
M2 (out in 0 0) 	 NMOS l=250n w=300n
ends inverter

//***** Power supply signals
Vdd (VDD 0) vsource dc=Supply

Va  (A 0) vsource type=pulse val0=0 val1=Supply period=perd\
         delay=pulse_delay rise=rise_time fall=fall_time width=high_time

//***** Instance of the inverter
I1 (A Y) 	inverter
C0 (Y 0)	capacitor c=load   // add load
```

O .mdl file:
```
alias measurement tp_meas {
	run tran(stop=4n)  // the analysis to run
	real rise_in=cross(sig=V(A), dir='rise, n=1, thresh=Supply/2, start=0)
	real fall_in=cross(sig=V(A), dir='fall, n=1, thresh=Supply/2, start=0)
	real fall_out=cross(sig=V(Y), dir='fall, n=1, thresh=Supply/2, start=0)
	real rise_out=cross(sig=V(Y), dir='rise, n=1, thresh=Supply/2, start=0)
	export real tphl=fall_out-rise_in   // the measurements
	export real tplh=rise_out-fall_in
	export real tp=(tphl+tplh)/2
}

foreach load from swp(start=1.5f, stop=7.5f, step=1.5f) {
	run tp_meas
}
```

## Medir propagation delay de NOR2 gate

Netlist NOR2:

```spectre
// Nor2
simulator lang=spice
.lib "45nm.lib" TT 
simulator lang=spectre

Settings options rawfmt=psfbin save=allpub
global 0 VDD
parameters	Wmin=90n  size=1
parameters	Supply=1.1
parameters  pulse_delay=1ns rise_time=100ps fall_time=100ps high_time=2ns perd=4.2ns

subckt nor2 inA inB out
// pull-up
M1 (out  inA midp VDD) PMOS l=50n w=size*Wmin
M2 (midp inB VDD VDD)	 PMOS l=50n w=size*Wmin
//pull-down
M3 (out inA 0 0) 	     NMOS l=50n w=90n
M4 (out inB 0 0) 	     NMOS l=50n w=90n
ends nor2

//***** Power supply signals
Vdd (VDD 0) vsource dc=Supply
Va  (A 0) vsource type=pulse val0=0 val1=Supply period=perd delay=pulse_delay rise=rise_time fall=fall_time width=high_time
Vb  (B 0) vsource dc=0

//***** Instance of the inverter
I1 (A B Y) 	nor2
I2 (Y 0)    capacitor c=4f

analysis tran stop=4ns
```

### Find the width of the PMOS transistors so that tpHL = tpLH, varying the input connected to the transistor closer to the output

```
alias measurement tp_meas {
    // Assume input B=0
	run tran(stop=4n)
	real rise_in=cross(sig=V(A), dir='rise, n=1, thresh=Supply/2, start=0)
	real fall_in=cross(sig=V(A), dir='fall, n=1, thresh=Supply/2, start=0)
	real fall_out=cross(sig=V(Y), dir='fall, n=1, thresh=Supply/2, start=0)
	real rise_out=cross(sig=V(Y), dir='rise, n=1, thresh=Supply/2, start=0)
	export real tphl=fall_out-rise_in
	export real tplh=rise_out-fall_in
	export real tp = (tphl+tplh)/2
	export real delta = abs(tphl-tplh)   // difference between the two delays
}

foreach size from swp(start=1, stop=10, step=0.1) {
	run tp_meas
}
```

### For this NOR2 gate (balanced output), find tpHL and tpLH for loads 1fF <= CL <= 20 fF (using the same inputs as before). From the measured values, determine the equation for the curve tpHL(CL) and comment. 

Primeiro temos de alterar o .scs (netlist) no M1 e M2 em vez de w=size*Wmin, colocamos o resultado que deu balanced. Para além disso, alteramos a capacitância no fim em vez de c=4f, metemos um parameter load.

```parameters load=4f``` e ```I2 (Y 0)    capacitor c=load```

Novo .mdl file:

```spectre
alias measurement tp_meas {
  // Assumes input B=0
	run tran(stop=4n)
	real rise_in=cross(sig=V(A), dir='rise, n=1, thresh=Supply/2, start=0)
	real fall_in=cross(sig=V(A), dir='fall, n=1, thresh=Supply/2, start=0)
	real fall_out=cross(sig=V(Y), dir='fall, n=1, thresh=Supply/2, start=0)
	real rise_out=cross(sig=V(Y), dir='rise, n=1, thresh=Supply/2, start=0)
	export real tphl=fall_out-rise_in
	export real tplh=rise_out-fall_in
	export real tp = (tphl+tplh)/2
}

foreach load from swp(start=1f, stop=20f, step=1f) {
	run tp_meas
}
```

# Lab 3
Assumir que temos o inversor, já balanced.

## Sem Carga, entao o delay mantem-se mesmo aumentando o tamanho dos transistors. delay =tp0 + RC, se C=0 entao mesmo que R diminua, o delay fica tp0

Nota: O ```m=mult``` no inverter quer dizer que vai colocar os NMOS e PMOS mult vezes maiores. Mas o rácio W/L mantem-se.

```spectre
// Investigating inverter chains

simulator lang=spectre
Settings options rawfmt=psfbin
simulatorOptions options  gmindc=1e-15

parameters Supply=2.5 mult=1
settings options save=all

global 0 VDD

subckt inverter in out
Mp1 out in VDD VDD PMOS w=772n l=250n
Mn0 out in 0 0     NMOS w=300n l=250n
ends inverter

Vdd (VDD 0) vsource dc=Supply

Vin (in 0) vsource type=pulse val0=0 val1=Supply width=2n period=4.2n rise=100p fall=100p delay=1n 

xinv (in out) inverter m=mult

simulator lang=spice
.lib "cmos250.lib" TT
simulator lang=spectre
```

.mdl file
```
alias measurement tp_meas {
	run tran (stop=10n)
	real rise_in=cross(sig=V(in), dir='rise, n=1, thresh=Supply/2, start=0)
	real fall_in=cross(sig=V(in), dir='fall, n=1, thresh=Supply/2, start=0)
	real fall_out=cross(sig=V(out), dir='fall, n=1, thresh=Supply/2, start=0)
	real rise_out=cross(sig=V(out), dir='rise, n=1, thresh=Supply/2, start=0)
	export real tphl=fall_out-rise_in
	export real tplh=rise_out-fall_in
    export real tp=(tphl+tplh)/2
}

foreach mult from  {1, 2, 4, 8, 16, 32} {
        run tp_meas  
}
```

## Encontrar o delay de um SSI (mult = 1) com uma carga de N SSIs com N {1,2,4,8,16,32}
Podemos colocar um inversor no fim do primeiro inversor e fazer um sweep pelo mult dele 

No .scs
```
xinv (in out) inverter m=1
xload (out dummy) inverter m=mult
```

No .mdl tenho de alterar o que exporto, porque com 2 inversores, o in vai ser igual ao out, por isso:

```
export real tphl=fall_out-fall_in
export real tplh=rise_out-rise_in
export real tp=(tphl+tplh)/2
```

## Inverter Chain com 2 inversores, primeiro é SSI m=1, o segundo m=mult. Total carga é 64 SSI, por isso meto um inversor no fim 64x maior que SSI, mult=64

```
xinv (in mid) inverter m=1
xinv2 (mid out) inverter m=mult
xload (out dummy) inverter m=64
```

.mdl (step 1 é opcional porque é 1 default)
```
foreach mult from 1 to 32 step 0.01 {
    run tp_chain
}
```

## 3 Inverter Chain

.scs
```
xinv  (in mid) inverter
xinv2 (mid mid2) inverter m=mult
xinv3 (mid2 out) inverter m=mult*mult
xinv_load (out dummy) inverter m=64
```

.mdl posso usar novamente o ```export real tphl=fall_out-rise_in``` e ```export real tplh=rise_out-fall_in```


## Pseudo NMOS

Define o circuito e faz uma sweep analysis:

```spectre
simulator lang=spectre
global 0 VDD

parameters Supply=2.5

Vdd (VDD 0) vsource dc=Supply
Vin (in 0) vsource dc=0

.subckt pseudo_inv in out
Mp1 out 0 VDD VDD pmos w=0.772u l=0.25u
Mn1 out in 0 0 nmos w=0.3u l=0.25u
.ends

Xinv (in out) pseudo_inv

.dc Vin 0 Supply 0.01
.settings options save=allpub currents=all
```

# Lab 4

## Use Spectre to find the size of the smallest symmetric inverter for the target technology. (Symmetric means that tphl ≈ tplh) Determine the parameter β. 

```spectre
// Find beta

include "/home/cadmgr/libs/gpdk045/v6.0/models/spectre/gpdk045.scs" section=mc

Settings options rawfmt=psfbin

// simulatorOptions options  gmindc=1e-16
parameters beta=1 supply=1.0
parameters pulse_delay=0.5ns rise_time=10ps fall_time=10ps high_time=1ns period=2.02ns

global 0 vdd!

M1 (Y A vdd! vdd!) g45p1svt l=45n w=beta*120n
M2 (Y A 0 0)       g45n1svt l=45n w=120n

load (Y 0) capacitor c=1f

Vdd (vdd! 0) vsource dc=1.0
Vin (A 0) vsource type=pulse val0=0 val1=supply period=period delay=pulse_delay rise=ris
e_time fall=fall_time width=high_time

// Analysis
curve tran start=0 stop=pulse_delay+2*period
```

```
alias measurement beta_meas {
	run tran(stop=pulse_delay+2*period)
    
	real rise_in =  cross(sig=V(A), dir='rise, n=1, thresh=supply/2, start=0)
	real fall_out = cross(sig=V(Y), dir='fall, n=1, thresh=supply/2, start=0)

    real fall_in  = cross(sig=V(A), dir='fall, n=1, thresh=supply/2, start=0)
	real rise_out = cross(sig=V(Y), dir='rise, n=1, thresh=supply/2, start=0)

    export real tphl = fall_out - rise_in
	export real tplh = rise_out - fall_in
}

mvarsearch {
	option {
		deltax = 1e-4
	   maxiter = 50
	 }
   parameter {
	   {beta, 2, 1, 8}
	 }
	exec {
	  run beta_meas
	}
	zero {
		delay_diff = abs(beta_meas->tphl - beta_meas->tplh)
	}
}
```

## Usar setup do stor de 4 stages, em cada stage para alem da gate tem outras 2. (desenhar grafico) O dut é o 3 inversor que está no stage 3. Obter os parametros p e g de d = f * g + p, tenho vários pontos. Declive é g, ordenada na origem é p

```
// 
simulator lang=spice
.lib "45nm.lib" TT
simulator lang=spectre

Settings options rawfmt=psfbin

// simulatorOptions options  gmindc=1e-16
parameters beta=1.82 supply=1.0 effective_fanout = 2
parameters pulse_delay=0.5ns rise_time=10ps fall_time=10ps high_time=1ns period=2.02ns

global 0 vdd!

subckt inverter in out
M1 (out in vdd! vdd!) g45p1svt l=45n w=beta*120n
M2 (out in 0 0)       g45n1svt l=45n w=120n
ends inverter

subckt gate in out
H  (in out) inverter 
ends gate

// stage
subckt stage in out dout
parameters f=2
G1 (in out)   gate
G2 (out mid)  gate   m=f-1
G3 (mid dout) gate   m=f
ends stage

s0   (A N D0)    stage f=effective_fanout
s1   (N X D1)    stage f=effective_fanout
dut  (X Y D2)    stage f=effective_fanout    // delay from X to Y 
s3   (Y Z D3)    stage f=effective_fanout

Vdd (vdd! 0) vsource dc=1.0
Vin (A 0) vsource type=pulse val0=0 val1=supply period=period delay=pulse_delay rise=ris
e_time fall=fall_time width=high_time

// Analysis
curve tran start=0 stop=pulse_delay+2*period
```

```
alias measurement delay_meas {
	run tran(stop=pulse_delay+2*period)
    
	real rise_in =  cross(sig=V(X), dir='rise, n=1, thresh=supply/2, start=0)
	real fall_out = cross(sig=V(Y), dir='fall, n=1, thresh=supply/2, start=0)

  real fall_in  = cross(sig=V(X), dir='fall, n=1, thresh=supply/2, start=0)
	real rise_out = cross(sig=V(Y), dir='rise, n=1, thresh=supply/2, start=0)

  export real tphl = fall_out - rise_in
	export real tplh = rise_out - fall_in
  export real tp = (tphl+tplh) / 2
}

foreach effective_fanout from swp(start=2, stop=10, step=0.1) {
        run delay_meas
}
```

## Delay para NOR2 gate usando o mesmo método

```
simulator lang=spectre
// adapt for your system
include "/home/jcf/CAD/tech/cds45nm/models/spectre/gpdk045.scs" section=mc

Settings options rawfmt=psfbin

simulatorOptions options  gmindc=1e-16
parameters beta=1.82 supply=1.0 effective_fanout = 2
parameters pulse_delay=0.5ns rise_time=10ps fall_time=10ps high_time=1ns period=2.02ns

global 0 vdd!

subckt inverter in out
M1 (out in vdd! vdd!) g45p1svt l=45n w=beta*120n
M2 (out in 0 0)       g45n1svt l=45n w=120n
ends inverter

subckt nor2 in out
M1 (mid 0  vdd! vdd!)    g45p1svt l=45n w=beta*120n*2
M2 (out in mid  vdd!)    g45p1svt l=45n w=beta*120n*2
M3 (out in 0 0)          g45n1svt l=45n w=120n
M4 (out 0  0 0)          g45n1svt l=45n w=120n
ends nand

subckt gate in out
H  (in out) nor2
ends gate

// stage
subckt stage in out dout
parameters f=2
G1 (in out)   gate
G2 (out mid)  gate   m=f-1
G3 (mid dout) gate   m=f
ends stage

s0   (A N D0)    stage f=effective_fanout
s1   (N X D1)    stage f=effective_fanout
dut  (X Y D2)    stage f=effective_fanout    // delay from X to Y 
s3   (Y Z D3)    stage f=effective_fanout

Vdd (vdd! 0) vsource dc=1.0
Vin (A 0) vsource type=pulse val0=0 val1=supply period=period delay=pulse_delay rise=rise_time fall=fall_time width=high_time

curve tran start=0 stop=pulse_delay+2*period
```

## Para outra gate qualquer, seria apenas mudar o subckt que é chamado nos stages

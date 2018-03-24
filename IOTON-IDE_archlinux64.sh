#!/bin/bash

# Script de instalacao do ambiente IoTon no ArchLinux_64
# Apenas uma copia do IOTON-IDE_linux64 com algumas adaptacoes necessarias ArchLinux

TEMP_FILES="/tmp/ioton-files-linux.tar.gz /tmp/linux-64"

# rotina para imprimir mensagem de erro
abort(){
	echo "
*****************
*** ABORTANDO ***
*****************

Um erro ocorreu, a instalacao NAO foi concluida com exito!
	"
	rm -rf $TEMP_FILES
	exit -1
}

# a funcao abort sera executada caso ocorra erro
trap abort ERR SIGTERM

# script eh abortador caso algum comando falhe
set -e

echo "AVISO: esse script necessita de permissoes root para executar alguns comandos. Verifique alguns comandos. Verifique seu conteudo para maiores detalhes."
sleep 5

# Editor de Texto Atom
echo "INSTALANDO EDITOR DE TEXTO ATOM"
# Verifica se o atom esta instalado.
# Caso nao estiver, ele eh instalado
if [ -f /bin/atom ] ; then
	echo "Atom ja esta instalado"
else
	sudo pacman -S atom
fi

# Packages ioton e platformio-ide e alguns necessarios
(set -x
 apm install ioton
 apm install platformio-ide
 
 if [ -f /bin/clang ] ; then
	 echo "clang ja esta instalado"
 else
	sudo pacman -S clang
 fi
 
 if [ -f /bin/rsync ] ; then
	 echo "rsync ja esta instalado"
 else
	 sudo pacman -S rsync
 fi
 )

# executar o Atom para que finalize a instalacao do platformio-ide
atom &
echo "AGUARDANDO FINALIZAR INSTALACAO"
while [ ! -f "$HOME/.platformio/penv/bin/pio" ] ; do
	:
done

# instalar o IOTON-API
echo "INSTALANDO BIBLIOTECAS DO IOTON"

$HOME/.platformio/penv/bin/pio lib --global install 313
wget https://raw.githubusercontent.com/iotontech/IOTON-IDE/master/linux-64.tar.gz -O /tmp/ioton-files-linux.tar.gz
tar -xzf /tmp/ioton-files-linux.tar.gz -C /tmp
sudo mv /tmp/linux-64/etc/udev/rules.d/* /etc/udev/rules.d/
rsync -a /tmp/linux-64/.icons $HOME
rsync -a /tmp/linux-64/.platformio $HOME
$HOME/.platformio/penv/bin/pio platform install ton
chmod +x $HOME/.platformio/packages/iotonConfig/iotonConfig
chmod +x $HOME/.platformio/packages/tool-dfu-util/*
mv /tmp/linux-64/iotonConfig.desktop $HOME/Desktop/
chmod +x $HOME/Desktop/iotonConfig.desktop

# remove arquivos temporarios
echo "REMOVENDO ARQUIVOS TEMPORARIOS"
rm -rf $TEMP_FILES

# w00t
echo "INSTALACAO FINALIZADA COM SUCESSO"
exit 0

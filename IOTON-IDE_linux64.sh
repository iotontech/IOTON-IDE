#!/bin/bash

TEMP_FILES="/tmp/ioton-files-linux.tar.gz /tmp/linux-64"

# rotina para imprimir mensagem de erro
abort()
{
    echo "
*****************
*** ABORTANDO ***
*****************

Um erro ocorreu, a instalacao NAO foi concluida com exito!
"
	rm -rf $TEMP_FILE
    exit -1
}

# a funcao abort sera executada caso ocorra erro
trap abort ERR SIGTERM

# script eh abortado caso algum comando falhe
set -e

echo "AVISO: esse script necessita de permissoes root para executar alguns comandos. Verifique seu conteudo para maiores detalhes.
"
sleep 5

# Editor de Texto Atom
echo "INSTALANDO EDITOR DE TEXTO ATOM"
# tenta instalar o atom caso ele ainda nao esteja
# caso ja esteja, ele eh atualizado
if ! (set -x; sudo apt-get install atom) ; then
    # caso a instalacao/atualizacao falhe, a devida ppa eh adicionada
    (set -x; sudo add-apt-repository ppa:webupd8team/atom)
    # a interrupcao do script em caso de erro eh cancelada
    trap - ERR SIGTERM; set +e
    # tenta instalar/atualizar o atom novamente
    (set -x
     sudo apt-get update
     sudo apt-get install atom)
    # armazena o valor de retorno do ultimo comando (sudo apt-get install atom)
    isAtomInstalled=$?
    # ativa novamente a interrupcao do script em caso de erro
    trap abort ERR SIGTERM; set -e
    # caso o atom nao tenha sido instalado, uma ultima tentativa de update e install eh realizada
    if [ $isAtomInstalled -ne 0 ]; then
        (set -x
         find /etc/apt/sources.list.d -iname webupd8team*.list -exec sudo apt-get update -o Dir::Etc::sourcelist="{}" -o Dir::Etc::sourceparts="-" -o APT::Get::List-Cleanup="0" \;
         sudo apt-get install atom)
    fi
fi

# Packages ioton e platformio-ide
echo "INSTALANDO PACKAGES DO ATOM"
(set -x
 apm install ioton
 apm install platformio-ide
 sudo apt-get install clang)

# executar o Atom para que finalize a instalação do platformio-ide
atom &
echo "AGUARDANDO FINALIZAR INSTALACAO"
while [ ! -f "$HOME/.platformio/penv/bin/pio" ]; do
	:
done

# instalar IOTON-API
echo "INSTALANDO BIBLIOTECAS DO IOTON"
(set -x
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
 chmod +x $HOME/Desktop/iotonConfig.desktop)

# remove arquivos temporarios
echo "REMOVENDO ARQUIVOS TEMPORARIOS"
(set -x; rm -rf $TEMP_FILES)

# w00t
echo "INSTALACAO FINALIZADA COM SUCESSO"
exit 0

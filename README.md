# DanOS

DanOS é um sistema operacional educacional simples, desenvolvido originalmente por [vinsdragonis](https://github.com/vinsdragonis/NoobOS) e expandido para incluir comandos adicionais que simulam funcionalidades do Ubuntu 24.04.

## Sobre

Este projeto é uma versão modificada e melhorada, com a adição de vários comandos básicos que simulam funcionalidades semelhantes às encontradas no Ubuntu 24.04. O objetivo é fornecer uma plataforma educacional para entender como os sistemas operacionais funcionam.

## Comandos Implementados

O sistema agora inclui os seguintes comandos:

- `mkdir`: Cria um novo diretório
- `rm`: Remove um arquivo
- `rm -r`: Remove um diretório recursivamente
- `nano`: Simula um editor de texto simples
- `cd`: Muda o diretório atual
- `lscpu`: Exibe informações sobre a CPU
- `free -h`: Exibe informações sobre o uso de memória
- `df -h`: Exibe informações sobre o uso de disco
- `speedtest`: Informações de rede

## Pré-requisitos

Para compilar e executar o DanOS, você precisará das seguintes dependências:

- NASM (Netwide Assembler)
- GCC (GNU Compiler Collection)
- QEMU (para emulação)
- GRUB e xorriso (para criar imagem ISO)

### Instalação

```bash
git clone https://github.com/pedrortxdev/DanOS.git
cd DanOS
```

### Compilação e Execução

1. Dê permissão de execução aos scripts:
   ```bash
   chmod +x build.sh fast_boot.sh direct_boot.sh create_iso.sh test_iso.sh
   ```

2. Escolha um método para executar o sistema:

#### Execução com QEMU

   **Inicialização normal:**
   ```bash
   ./build.sh
   ```
   
   **Inicialização ultra-rápida:**
   ```bash
   ./fast_boot.sh
   ```
   
   **Inicialização direta:**
   ```bash
   ./direct_boot.sh
   ```

#### Execução com VirtualBox

1. Crie uma imagem ISO bootável:
   ```bash
   ./create_iso.sh
   ```

2. Teste a imagem ISO com QEMU (opcional):
   ```bash
   ./test_iso.sh
   ```

3. Configure uma máquina virtual no VirtualBox:
   - Abra o VirtualBox
   - Clique em 'Novo'
   - Dê um nome como "DanOS"
   - Selecione 'Tipo: Other' e 'Versão: Other/Unknown'
   - Aloque 64MB de memória
   - Não crie um disco rígido virtual
   - Após criar a VM, clique em 'Configurações'
   - Vá para 'Armazenamento'
   - Adicione a imagem ISO "danos.iso" ao controlador IDE
   - Inicie a VM

O script irá compilar o código-fonte (se necessário) e iniciar o sistema operacional. A saída será exibida no terminal ou na janela da máquina virtual.

## Métodos de Inicialização

O DanOS oferece três métodos de inicialização com QEMU, cada um com suas vantagens:

1. **Inicialização normal (`build.sh`)**: 
   - Simula um processo de inicialização completo
   - Útil para entender como um sistema operacional real é inicializado
   - Mais lento, mas mais realista

2. **Inicialização ultra-rápida (`fast_boot.sh`)**:
   - Pula algumas etapas do processo de inicialização
   - Mais rápido que o método normal
   - Ainda pode mostrar a mensagem "Booting from ROM..." por algum tempo

3. **Inicialização direta (`direct_boot.sh`)**:
   - Usa configurações mínimas do QEMU
   - Redireciona a saída diretamente para o terminal
   - A maneira mais rápida e confiável de iniciar o sistema

4. **Execução com VirtualBox (`create_iso.sh`)**:
   - Cria uma imagem ISO bootável
   - Permite executar o sistema em um ambiente de máquina virtual completo
   - Útil para demonstrações e testes em diferentes ambientes

## Características

O DanOS é um sistema operacional minimalista que:

1. Inicializa um ambiente básico
2. Exibe uma interface de linha de comando simples
3. Permite a execução de comandos básicos
4. Simula um sistema de arquivos e operações de hardware

Ao iniciar, o sistema executa automaticamente uma sequência de comandos de demonstração para mostrar as funcionalidades implementadas.

## Detalhes Técnicos

### Sistema de Arquivos Simulado

O sistema de arquivos é simulado em memória e inclui:
- Estrutura de diretórios hierárquica
- Suporte para arquivos e diretórios
- Operações básicas (criar, remover, navegar)

### Implementação dos Comandos

Cada comando é implementado como uma função em C que simula o comportamento do comando correspondente no Ubuntu:

- `mkdir`: Cria uma entrada de diretório no sistema de arquivos simulado
- `rm`: Remove uma entrada de arquivo do sistema de arquivos simulado
- `cd`: Atualiza o diretório atual no sistema
- `lscpu`, `free -h`, `df -h`: Exibem informações simuladas sobre o hardware

### Limitações

Este é um sistema educacional com várias limitações:
- Não há suporte para multitarefa
- Não há sistema de arquivos persistente
- Não há suporte para dispositivos reais
- A interação com o usuário é limitada

## Contribuições

Contribuições são bem-vindas! Sinta-se à vontade para fazer um fork do repositório, implementar melhorias e enviar um pull request.

Algumas ideias para melhorias:
- Adicionar mais comandos (ls, cat, echo, etc.)
- Implementar um sistema de arquivos persistente
- Adicionar suporte para entrada do teclado em tempo real
- Melhorar a interface do usuário

## Licença

Este projeto mantém a licença original do projeto base (MIT).

## Créditos

- [vinsdragonis](https://github.com/vinsdragonis) pelo projeto base
- Contribuidores que ajudaram a expandir e melhorar o sistema

## Agradecimentos

- Comunidade de desenvolvimento de sistemas operacionais por recursos educacionais

## Referências

- [VGA text mode](https://en.wikipedia.org/wiki/VGA_text_mode)
- [Multiboot](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
- [OSdev Wiki](https://wiki.osdev.org/Main_Page)
- [OS tutorial by cfenollosa](https://github.com/cfenollosa/os-tutorial)

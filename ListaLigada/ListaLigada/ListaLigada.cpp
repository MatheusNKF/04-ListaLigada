
#include <iostream>
#include <cstdlib>   // malloc, free e system
using namespace std;

// ---------------------------------------------------------------------------
// Definição de tipo: cada nó ("post-it") guarda um valor e o endereço do
// próximo nó ("a seta"). No último nó, prox vale NULL.
// ---------------------------------------------------------------------------
struct NO {
	int valor;
	NO* prox;
};

// Ponto de entrada da lista. NULL significa lista vazia.
// Atenção: se você perder este ponteiro, perde a lista inteira.
NO* primeiro = NULL;

// headers
void menu();
void inicializar();
void liberarLista();
void exibirQuantidadeElementos();
void exibirElementos();
void inserirElemento();
void excluirElemento();
void buscarElemento();
NO* posicaoElemento(int numero);
void limparTela();
void pausar();
//--------------------------


int main()
{
	menu();
}

void menu()
{
	int op = 0;
	while (op != 7) {
		limparTela();
		cout << "Menu Lista Ligada";
		cout << endl << endl;
		cout << "1 - Inicializar Lista \n";
		cout << "2 - Exibir quantidade de elementos \n";
		cout << "3 - Exibir elementos \n";
		cout << "4 - Buscar elemento \n";
		cout << "5 - Inserir elemento \n";
		cout << "6 - Excluir elemento \n";
		cout << "7 - Sair \n\n";

		cout << "Opcao: ";
		if (!(cin >> op)) {
			// entrada inválida (ex.: uma letra) ou fim da entrada
			if (cin.eof()) {
				op = 7;
			}
			else {
				cin.clear();              // limpa o estado de erro do cin
				cin.ignore(10000, '\n');  // descarta o que foi digitado
				op = 0;
			}
		}

		switch (op)
		{
		case 1: inicializar();
			break;
		case 2: exibirQuantidadeElementos();
			break;
		case 3: exibirElementos();
			break;
		case 4: buscarElemento();
			break;
		case 5: inserirElemento();
			break;
		case 6: excluirElemento();
			break;

		case 7:
			// devolve ao sistema a memória de todos os nós antes de sair
			liberarLista();
			return;
		default:
			cout << "Opcao invalida \n";
			break;
		}

		pausar();
	}
}

// Percorre a lista liberando (free) cada nó e deixa a lista vazia.
// Usada por inicializar() e ao sair do programa, para não haver
// vazamento de memória.
void liberarLista()
{
	NO* aux = primeiro;
	while (aux != NULL) {
		NO* paraExcluir = aux;   // guarda o endereço do nó atual...
		aux = aux->prox;         // ...avança ANTES de liberar...
		free(paraExcluir);       // ...e só então libera o nó
	}
	primeiro = NULL;
}

void inicializar()
{
	// se a lista já possuir elementos, libera a memória ocupada
	liberarLista();
	cout << "Lista inicializada \n";
}

void exibirQuantidadeElementos() {

	int nElementos = 0;
	NO* aux = primeiro;
	while (aux != NULL) {
		nElementos++;
		aux = aux->prox;
	}
	cout << "Quantidade de elementos: " << nElementos << endl;

}

void exibirElementos()
{
	if (primeiro == NULL) {
		cout << "Lista vazia \n";
		return;
	}
	else {
		cout << "Elementos: \n";
		NO* aux = primeiro;
		while (aux != NULL) {
			cout << aux->valor << endl;
			aux = aux->prox;
		}
	}
}

void inserirElemento()
{
	// Declara uma caixinha temporária de número inteiro para guardar o valor digitado
	int valor;
	cout << "Digite o elemento: ";
	cin >> valor;

	// TAREFA 1: Usa a função posicaoElemento para ver se esse número já está na lista

	if (posicaoElemento(valor) != NULL) {

		// 2) ...e só depois alocamos memória para o novo nó
		cout << "Elemento já existe na lista, insercao cancelada\n";
		return;
	}

	NO* novo = (NO*)malloc(sizeof(NO));

	// Verifica se o computador ficou sem memória (se o malloc retornou NULL)
	if (novo == NULL) {
		cout << "Memoria insuficiente \n";
		return; // Sai da função se não houver memória
	}

	// Guarda o número digitado dentro do campo 'valor' da nossa nova caixinha

	novo->valor = valor;
	novo->prox = NULL;

	// Pergunta: o ponteiro 'primeiro' está apontando para NULL? (Lista vazia?)
	if (primeiro == NULL) {

		// Se a lista estiver vazia, o nosso 'novo' nó passa a ser o primeiro da lista
		primeiro = novo;
	}
	else
	{
		// procura o final da lista (o nó cujo prox é NULL)
		NO* aux = primeiro;
		while (aux->prox != NULL) {
			aux = aux->prox;
		}
		aux->prox = novo;
	}
}

void excluirElemento()
{
	// 1. Cria uma variável para guardar o número que o usuário quer apagar
	int valor;
	cout << "Digite o elemento que deseja excluir: ";
	cin >> valor;

	// 2. Verifica se o elemento realmente existe usando posicaoElemento()[cite: 4]
	if (posicaoElemento(valor) == NULL) {
		// Se retornar NULL, o elemento não está na lista[cite: 4]
		cout << "ELEMENTO NAO ENCONTRADO\n";
		return; // Sai da função
	}

	// 3. CASO A: O elemento a ser excluído é o PRIMEIRO da lista
	if (primeiro->valor == valor) {
		// Guarda o endereço do primeiro nó temporariamente para não perder
		NO* paraExcluir = primeiro;

		// O ponteiro 'primeiro' pula para o segundo nó da lista (primeiro->prox)
		primeiro = primeiro->prox;

		// Libera a memória do nó antigo que estava na frente[cite: 5]
		free(paraExcluir);

		cout << "Elemento excluido com sucesso!\n";
		return;
	}

	// 4. CASO B: O elemento está no MEIO ou no FIM da lista[cite: 5]
	// Precisamos de dois ponteiros: 'anterior' e 'atual'[cite: 5]
	NO* anterior = primeiro;
	NO* atual = primeiro->prox; // 'atual' começa no segundo elemento

	// Percorre a lista até achar o elemento que queremos excluir[cite: 5]
	while (atual != NULL && atual->valor != valor) {
		anterior = atual;       // O 'anterior' acompanha o 'atual'
		atual = atual->prox;    // O 'atual' avança para o próximo
	}

	// Se o 'atual' encontrou o valor procurado:
	if (atual != NULL) {
		// Religa a lista: o 'anterior->prox' pula o 'atual' e aponta direto para o 'atual->prox'[cite: 5]
		anterior->prox = atual->prox;

		// Agora que o nó 'atual' está isolado, liberamos a memória dele com free[cite: 5]
		free(atual);

		cout << "Elemento excluido com sucesso!\n";
	}
}

void buscarElemento()
{
	// -----------------------------------------------------------------
	// TAREFA 2
	// 1. Peça ao usuário o número a ser buscado.
	// 2. Chame posicaoElemento(numero).
	// 3. Retorno diferente de NULL -> exiba "ENCONTRADO"
	//    Retorno igual a NULL     -> exiba "ELEMENTO NAO ENCONTRADO"
	// -----------------------------------------------------------------
}



// retorna um ponteiro para o elemento buscado
// ou NULL se o elemento não estiver na lista
NO* posicaoElemento(int numero)
{
	NO* aux = primeiro;
	while (aux != NULL) {
		if (aux->valor == numero)
		{
			break;
		}
		aux = aux->prox;
	}
	return aux;
}

// ---------------------------------------------------------------------------
// Utilitários de tela: funcionam no Windows (Visual Studio) e também no
// Linux/macOS (g++ ou clang++).
// ---------------------------------------------------------------------------
void limparTela()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

void pausar()
{
#ifdef _WIN32
	system("pause");
#else
	cout << "Pressione ENTER para continuar...";
	cin.ignore(10000, '\n');
	cin.get();
#endif
}

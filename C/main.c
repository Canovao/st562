#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

char directory_name[128]; // comprimento máximo de 128 caracteres

char *view[] = {
    "-----",
    "-",
    "   -----",
    "   ",
    "    -----"};

void TREE_TRAVERSE(const char *path);
int FIND_FILE(const char *path, const char *filename, char *found_path);
void DELETE_FILE(const char *path);
void CREATE_FILE(const char *filename);
void RESIZE_FILE(const char *filename, size_t new_size);
int counter, sw = 0;
char current_working_directory[128];
// TODO criar função para criar um folder
// TODO criar função para deletar um folder
// TODO criar uma função para entrar num subfolder
// TODO criar uma função para sair de um subfolder
// TODO melhorar o print para mostrar o tamanho do arquivo

int main()
{
    char user_input[128];
    int choice;

    // Obter o diretório atual
    GetCurrentDirectoryA(sizeof(current_working_directory), current_working_directory);

    // Solicitar ao usuário que insira o nome do diretório
    printf("Digite o nome do diretório (use duas barras invertidas, por exemplo, C:\\\\Users\\\\usuario\\\\Downloads): ");
    fgets(user_input, sizeof(user_input), stdin);

    // Remover o caractere de nova linha, se presente
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n')
    {
        user_input[len - 1] = '\0';
    }

    // Alterar para o diretório fornecido pelo usuário
    if (!SetCurrentDirectoryA(user_input))
    {
        printf("Erro: Não foi possível acessar o diretório especificado.\n");
        return 1;
    }

    while (TRUE)
    {
        // Solicitar a escolha do usuário
        printf("Escolha uma opção:\n1 - Varrer o diretório\n2 - Procurar por um arquivo\n3 - Deletar um arquivo\n4 - Criar um arquivo\n5 - Redimensionar um arquivo\n6 - Sair do programa\nDigite sua escolha: ");
        scanf("%d", &choice);
        getchar(); // Consumir o caractere de nova linha

        if (choice == 1)
        {
            // Varrer o diretório
            TREE_TRAVERSE("*.*");
        }
        else if (choice == 2)
        {
            // Procurar por um arquivo
            char filename[128];
            char found_path[256] = "";
            printf("Digite o nome do arquivo que deseja procurar: ");
            fgets(filename, sizeof(filename), stdin);

            // Remover o caractere de nova linha, se presente
            len = strlen(filename);
            if (len > 0 && filename[len - 1] == '\n')
            {
                filename[len - 1] = '\0';
            }

            // Procurar pelo arquivo
            if (FIND_FILE("*.*", filename, found_path))
            {
                printf("Arquivo '%s' encontrado no caminho: %s\n", filename, found_path);
            }
            else
            {
                printf("Arquivo '%s' não encontrado.\n", filename);
            }
        }
        else if (choice == 3)
        {
            // Deletar um arquivo
            char filename[128];
            char found_path[256] = "";
            printf("Digite o nome do arquivo que deseja deletar: ");
            fgets(filename, sizeof(filename), stdin);

            // Remover o caractere de nova linha, se presente
            len = strlen(filename);
            if (len > 0 && filename[len - 1] == '\n')
            {
                filename[len - 1] = '\0';
            }

            // Procurar pelo arquivo
            if (FIND_FILE("*.*", filename, found_path))
            {
                printf("Arquivo '%s' encontrado no caminho: %s\n", filename, found_path);
                char confirm[4];
                printf("Tem certeza que deseja deletar este arquivo? (Sim/Não): ");
                fgets(confirm, sizeof(confirm), stdin);
                // Remover o caractere de nova linha, se presente
                len = strlen(confirm);
                if (len > 0 && confirm[len - 1] == '\n')
                {
                    confirm[len - 1] = '\0';
                }
                // Verificar confirmação
                if (strcasecmp(confirm, "Sim") == 0)
                {
                    DELETE_FILE(found_path);
                    printf("Arquivo '%s' deletado com sucesso.\n", filename);
                }
                else
                {
                    printf("Deleção cancelada.\n");
                }
            }
            else
            {
                printf("Arquivo '%s' não encontrado.\n", filename);
            }
        }
        else if (choice == 4)
        {
            // Criar um arquivo
            char filename[128];
            printf("Digite o nome do arquivo que deseja criar (sem extensão): ");
            fgets(filename, sizeof(filename), stdin);

            // Remover o caractere de nova linha, se presente
            len = strlen(filename);
            if (len > 0 && filename[len - 1] == '\n')
            {
                filename[len - 1] = '\0';
            }

            // Criar o arquivo com extensão .txt
            char full_filename[256];
            snprintf(full_filename, sizeof(full_filename), "%s.txt", filename);
            CREATE_FILE(full_filename);
            printf("Arquivo '%s' criado com sucesso.\n", full_filename);
        }
        else if (choice == 5)
        {
            // Redimensionar um arquivo
            char filename[128];
            printf("Digite o nome do arquivo que deseja redimensionar: ");
            fgets(filename, sizeof(filename), stdin);

            // Remover o caractere de nova linha, se presente
            len = strlen(filename);
            if (len > 0 && filename[len - 1] == '\n')
            {
                filename[len - 1] = '\0';
            }

            size_t new_size;
            printf("Digite o novo tamanho do arquivo (em bytes): ");
            scanf("%zu", &new_size);

            // Redimensionar o arquivo
            RESIZE_FILE(filename, new_size);
            printf("Arquivo '%s' redimensionado para %zu bytes.\n", filename, new_size);
        }
        else if (choice == 6)
        {
            break;
        }
        else
        {
            printf("Opção inválida.\n");
        }
    }

    // Voltar para o diretório original
    SetCurrentDirectoryA(current_working_directory);

    return 0;
}

void TREE_TRAVERSE(const char *path)
{
    WIN32_FIND_DATA find_data;
    HANDLE hFind;
    static int deep_inside = 0;

    // Encontrar o primeiro arquivo/diretório
    hFind = FindFirstFile(path, &find_data);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return; // Nenhum arquivo ou diretório encontrado
    }

    do
    {
        // Preparar o nome para impressão com indentação
        directory_name[0] = '\0';
        if (deep_inside == 0)
        {
            strcat(directory_name, view[0]);
        }
        else
        {
            strcat(directory_name, view[1]);
            for (int i = 1; i < deep_inside; i++)
            {
                strcat(directory_name, view[3]);
            }
            strcat(directory_name, view[2]);
        }
        strcat(directory_name, find_data.cFileName);

        // Verificar se é um diretório
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // Ignorar "." e ".."
            if (strcmp(find_data.cFileName, ".") != 0 && strcmp(find_data.cFileName, "..") != 0)
            {
                printf("%s (Pasta)\n", directory_name); // Imprimir o nome do diretório

                // Entrar no diretório e realizar a travessia recursiva
                deep_inside++;
                char next_path[256];
                snprintf(next_path, sizeof(next_path), "%s\\%s", path, find_data.cFileName);
                SetCurrentDirectoryA(find_data.cFileName);
                TREE_TRAVERSE("*.*");

                // Voltar para o diretório anterior
                SetCurrentDirectoryA("..");
                deep_inside--;
            }
        }
        else
        {
            // Imprimir o nome do arquivo
            printf("%s\n", directory_name);
        }
    } while (FindNextFile(hFind, &find_data) != 0);

    FindClose(hFind);
}

int FIND_FILE(const char *path, const char *filename, char *found_path)
{
    WIN32_FIND_DATA find_data;
    HANDLE hFind;
    static int deep_inside = 0;

    // Encontrar o primeiro arquivo/diretório
    hFind = FindFirstFile(path, &find_data);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return 0; // Nenhum arquivo ou diretório encontrado
    }

    do
    {
        // Verificar se é o arquivo procurado
        if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmp(find_data.cFileName, filename) == 0)
        {
            // Montar o caminho completo do arquivo encontrado
            GetCurrentDirectoryA(256, found_path);
            strcat(found_path, "\\");
            strcat(found_path, find_data.cFileName);
            FindClose(hFind);
            return 1; // Arquivo encontrado
        }

        // Se for um diretório, entrar recursivamente
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (strcmp(find_data.cFileName, ".") != 0 && strcmp(find_data.cFileName, "..") != 0)
            {
                deep_inside++;
                SetCurrentDirectoryA(find_data.cFileName);
                if (FIND_FILE("*.*", filename, found_path))
                {
                    FindClose(hFind);
                    return 1; // Arquivo encontrado em um subdiretório
                }
                SetCurrentDirectoryA("..");
                deep_inside--;
            }
        }
    } while (FindNextFile(hFind, &find_data) != 0);

    FindClose(hFind);
    return 0; // Arquivo não encontrado
}

void DELETE_FILE(const char *path)
{
    // Deletar o arquivo especificado
    DeleteFileA(path);
}

void CREATE_FILE(const char *filename)
{
    // Criar o arquivo especificado
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_WRITE,
        0,          // Não permite acesso compartilhado
        NULL,       // Atributos de segurança padrão
        CREATE_NEW, // Cria um novo arquivo; falha se o arquivo já existir
        FILE_ATTRIBUTE_NORMAL,
        NULL // Sem template
    );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("Erro ao criar o arquivo: %d\n", GetLastError());
        return;
    }

    // Fechar o arquivo após a criação
    CloseHandle(hFile);
}

void RESIZE_FILE(const char *filename, size_t new_size)
{
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("Erro ao abrir o arquivo para redimensionar: %d\n", GetLastError());
        return;
    }

    // Redimensionar o arquivo
    if (SetFilePointer(hFile, new_size, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        printf("Erro ao definir o ponteiro do arquivo.\n");
        CloseHandle(hFile);
        return;
    }

    if (!SetEndOfFile(hFile))
    {
        printf("Erro ao redimensionar o arquivo.\n");
    }

    CloseHandle(hFile);
}

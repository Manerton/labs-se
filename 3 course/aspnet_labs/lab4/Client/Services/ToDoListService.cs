using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Net.Http.Json;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using Lab4.Shared.Domain;
using Lab4.Shared.DTO;

namespace Lab4.Client.Services
{
    public class ToDoListService
    {
        private readonly HttpClient _http;

        public ToDoListService(HttpClient http)
        {
            _http = http;
        }
        public async Task<IEnumerable<ToDoListModel>> GetToDoList()
        {
            
            return await _http.GetFromJsonAsync<IEnumerable<ToDoListModel>>("api/to-do-list");
        }

        public async Task<ToDoListModel> GetToDoListById(string id)
        {
            return await _http.GetFromJsonAsync<ToDoListModel>($"api/to-do-list/{id}");
        }

        public async Task<string> GetNameOfList(string id)
        {
            ToDoListModel list = await GetToDoListById(id);
            return list.Name;
        }

        public async Task CreateToDoList(CreateToDoListDTO toDoList)
        {
            var result = await _http.PostAsJsonAsync($"api/to-do-list", toDoList);

            if (!result.IsSuccessStatusCode)
                throw new ApplicationException("Не получилось добавить список.");
        }

        public async Task DeleteToDoList(ToDoListModel toDoList)
        {
            var result = await _http.DeleteAsync($"api/to-do-list?id={toDoList.ListId}");

            if (!result.IsSuccessStatusCode)
                throw new ApplicationException("Не получилось удалить список.");
        }

        public async Task UpdateToDoList(ToDoListModel toDoList)
        {
            var result = await _http.PatchAsync($"api/to-do-list",
                new StringContent(JsonSerializer.Serialize(toDoList),
                Encoding.UTF8,
                "application/json"));


            if (!result.IsSuccessStatusCode)
                throw new ApplicationException("Не получилось обновить список.");
        }
    }
}

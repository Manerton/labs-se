using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Lab4.Server.Data;
using Lab4.Shared.Domain;
using Microsoft.EntityFrameworkCore;

namespace Lab4.Server.Repositories
{
    public class ToDoListRepository
    {
        private ApplicationDbContext _context;
        public ToDoListRepository(ApplicationDbContext context)
        {
            _context = context;
        }
        public async Task<IList<ToDoListModel>> List(string Id)
        {
            return await _context.Lists
                .OrderBy(x => x.ListId)
                .Where(x => x.UserId == Id)
                .ToListAsync();
        }
        public async Task Add(ToDoListModel data)
        {
            await _context.Lists.AddAsync(data);
            await _context.SaveChangesAsync();
        }
        public async Task Update(ToDoListModel data)
        {
            // Find data with same id
            ToDoListModel updatingData = _context.Lists
                .Single(x => x.ListId == data.ListId);
            // Update it's attributes
            updatingData.Name = data.Name;
            await _context.SaveChangesAsync();
        }
        public async Task Remove(Guid Id)
        {
            // Find data with same id
            ToDoListModel removingData = _context.Lists
                .Single(x => x.ListId == Id);
            // Remove data row
            _context.Lists.Remove(removingData);
            await _context.SaveChangesAsync();
        }
        public async Task<ToDoListModel> GetItemById(Guid Id)
        {
            // Find data with same id
            return await _context.Lists
                .SingleAsync(x => x.ListId == Id);
        }
    }
}

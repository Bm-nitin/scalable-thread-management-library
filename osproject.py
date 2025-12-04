import threading
from queue import Queue
import time
import tkinter as tk
from tkinter import messagebox

class ThreadPool:
    def __init__(self, num_threads):
        self.tasks = Queue()
        self.stop = False
        self.workers = []

        for i in range(num_threads):
            t = threading.Thread(target=self.worker, daemon=True)
            t.start()
            self.workers.append(t)

    def worker(self):
        while not self.stop:
            task = self.tasks.get()
            if task is None:
                break
            task()
            self.tasks.task_done()

    def add_task(self, func):
        self.tasks.put(func)

    def shutdown(self):
        self.stop = True
        for _ in self.workers:
            self.tasks.put(None)
        for t in self.workers:
            t.join()


# -------- GUI PART --------

class GUI:
    def __init__(self, pool):
        self.pool = pool
        self.window = tk.Tk()
        self.window.title("Thread Manager - Version 1 GUI")
        self.window.geometry("400x300")
        
        self.label = tk.Label(self.window, text="Thread Manager Running", font=("Arial", 14))
        self.label.pack(pady=20)

        self.task_button = tk.Button(self.window, text="Add Task", font=("Arial", 12),
                                     bg="#4CAF50", fg="white", width=15, command=self.add_task)
        self.task_button.pack(pady=10)

        self.window.protocol("WM_DELETE_WINDOW", self.on_close)

    def add_task(self):
        task_id = int(time.time() * 1000) % 10000
        self.pool.add_task(lambda n=task_id: print(f"Task {n} executed by thread {threading.get_ident()}"))

        messagebox.showinfo("Task Added", "Task successfully added to thread pool!")

    def on_close(self):
        self.pool.shutdown()
        self.window.destroy()

    def run(self):
        self.window.mainloop()


# -------- MAIN PROGRAM --------

if __name__ == "__main__":
    pool = ThreadPool(4)
    gui = GUI(pool)
    gui.run()

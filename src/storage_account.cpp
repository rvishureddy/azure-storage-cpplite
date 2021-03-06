#include "storage_account.h"

#include "constants.h"

namespace azure {  namespace storage_lite {

    std::shared_ptr<storage_account> storage_account::development_storage_account()
    {
        std::string account_name = "devstoreaccount1";
        std::string account_key = "Eby8vdM02xNOcqFlqUwJPLlmEtlCDXJ1OUzFT50uSRZ6IFsuFq2UVErCz4I6tq/K1SZFPTOtr/KBHBeksoGMGw==";
        std::shared_ptr<storage_credential>  cred = std::make_shared<shared_key_credential>(account_name, account_key);
        std::shared_ptr<storage_account> account = std::make_shared<storage_account>(account_name, cred, false, "127.0.0.1:10000/devstoreaccount1");
        return account;
    }

    // TODO: Clean up table queue and file services
    storage_account::storage_account(const std::string &account_name, std::shared_ptr<storage_credential> credential, bool use_https, const std::string &blob_endpoint)
        : m_credential(credential)
    {
        if (use_https)
        {
            append_all("https://");
        }
        else
        {
            append_all("http://");
        }

        if(blob_endpoint.empty())
        {
            append_all(account_name);

            m_blob_domain.append(".blob");
            m_table_domain.append(".table");
            m_queue_domain.append(".queue");
            m_file_domain.append(".file");

            append_all(constants::default_endpoint_suffix);
        }
        else
        {
            append_all(blob_endpoint);
        }
    }

    AZURE_STORAGE_API storage_url storage_account::get_url(service service) const
    {
        storage_url url;
        switch (service)
        {
        case storage_account::service::blob:
            url.set_domain(m_blob_domain);
            break;
        case storage_account::service::table:
            url.set_domain(m_table_domain);
            break;
        case storage_account::service::queue:
            url.set_domain(m_queue_domain);
            break;
        case storage_account::service::file:
            url.set_domain(m_file_domain);
            break;
        }

        return url;
    }

    AZURE_STORAGE_API void storage_account::append_all(const std::string &part)
    {
        m_blob_domain.append(part);
        m_table_domain.append(part);
        m_queue_domain.append(part);
        m_file_domain.append(part);
    }

}}
